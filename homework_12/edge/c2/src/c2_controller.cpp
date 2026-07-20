#include "c2_controller.hpp"
#include "fc_link.hpp"     // MAVSDK обгортка, API описано у fc_link.hpp
#include "udp_socket.hpp"  // UDP прийом, API описано у udp_socket.hpp

#include <cstdint>
#include <nlohmann/json.hpp>  // Розбiр JSON з точками маршруту вiд auto_stub

#include <fstream>
#include <iostream>
#include <string>

static constexpr uint16_t STUB_PORT = 14560;

std::string to_string (C2State state) {
    switch (state) {
        case C2State::DISARMED : return "DISARMED";
        case C2State::ARMED_HOLD : return "ARMED_HOLD";
        case C2State::ARMED_GUIDED : return "ARMED_GUIDED";
        case C2State::ARMED_MANUAL : return "ARMED_MANUAL";
    }
    return "UNKNOWN";
}

struct C2Controller::Impl {
    C2State state = C2State::DISARMED;

    // TODO: додати FcLink, UdpSocket, лог-файл та прапорцi стану.
    // FcLink потребує fc_port у конструкторi Impl.
    // UdpSocket має слухати STUB_PORT.


    FcLink fc;
    UdpSocket socket;
    std::ofstream log_file;
    bool healthy_written;

    explicit Impl(uint16_t fc_port) :
    fc(fc_port), socket(STUB_PORT), log_file("/var/log/c2/c2.log"), healthy_written (false){}

    

    void transition(C2State next) {
        // TODO: якщо next != state, записати "PREV -> NEW" у stdout i лог,
        // потiм оновити state. Якщо стан не змiнився, нiчого не писати.
        if(next != state){
            std::string old_name = to_string(state);
            std::string new_name = to_string(next);

            std::cout << "[C2] state: " << old_name << " -> " << new_name << std::endl;

            if(log_file.is_open()) {
                log_file << "[C2] state: " << old_name << " -> " << new_name << std::endl;
            }

            state = next;

            if(next == C2State::ARMED_HOLD) {
                fc.hold();
            }
        }
    }
};

C2Controller::C2Controller(uint16_t fc_port)
    : impl_(std::make_unique<Impl>(fc_port))
{
    // TODO: передати fc_port в Impl та вiдкрити /var/log/c2/c2.log.
}

C2Controller::~C2Controller() = default;

void C2Controller::tick() {
    // TODO: healthcheck, оновлення C2State, читання точки маршруту,
    // передавання або блокування команди згiдно з поточним станом.
    if (!impl_->healthy_written && impl_->fc.is_connected()){
        std::ofstream("/tmp/c2_healthy");
        impl_->healthy_written = true;
    }

    C2State next = impl_->state;
    if(!impl_->fc.is_armed()){
        next = C2State::DISARMED;
    } else {
         switch(impl_->fc.flight_mode()){
            case FcLink::FlightMode::Guided : next = C2State::ARMED_GUIDED;
             break;
            case FcLink::FlightMode::Hold : next = C2State::ARMED_HOLD;
             break;
            case FcLink::FlightMode::Manual  : next = C2State::ARMED_MANUAL;
             break;
            case FcLink::FlightMode::Unknown  : next = C2State::ARMED_MANUAL;
             break;
        }
    }
    
    impl_->transition(next);

    char buffer[256];
    ssize_t n = impl_->socket.recv(buffer, sizeof(buffer));
    if(n > 0){
        std::string json_str(buffer, n);
        auto j = nlohmann::json::parse(json_str);
        if(impl_->state == C2State::ARMED_GUIDED) {
            float north = j.at("north_m").get<float>();
            float east = j.at("east_m").get<float>();

            impl_->fc.go_to_ned(north, east);

            impl_->log_file << "[C2] fwd: north=" << north << " east=" << east << std::endl;
            std::cout << "[C2] fwd: north=" << north << " east=" << east << std::endl;
        } else {
            impl_->log_file << "[C2] blocked: waypoint in " << to_string(impl_->state) << std::endl;
            std::cout << "[C2] blocked: waypoint in " << to_string(impl_->state) << std::endl;
        }

    }
    
}

C2State C2Controller::current_state() const {
    return impl_->state;
}
