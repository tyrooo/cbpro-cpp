//
// Created by Justin Barnett on 4/4/21.
//

#ifndef TRADINGSYSTEMS_WEBSOCKETCLIENT_H
#define TRADINGSYSTEMS_WEBSOCKETCLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <set>
#include <thread>

#include "../websocket/channel.h"

#define COINBASE_WEBSOCKET_URI "wss://ws-feed.pro.coinbase.com"

namespace pt = boost::property_tree;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

namespace websocket {
    namespace client {
        typedef websocketpp::client<websocketpp::config::asio_client> client;
        typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

        class WebSocketClient {
        public:
            static websocket::client::client singletonClient;
            static bool connected;
            static std::string pendingSubscribeMessage;
            static websocketpp::connection_hdl serverConnection;

            std::thread* clientThread;

            bool subscribe(const std::vector<websocket::models::channel> channels,
                           const std::vector<std::string> products);
        private:
            void sendMessage(const pt::ptree message);
        };

        std::string toJsonString(const pt::ptree propertyTree) {
            std::stringstream ss;
            pt::json_parser::write_json(ss, propertyTree);
            return ss.str();
        }

        void onMessage(websocket::client::client* client, websocketpp::connection_hdl hdl, message_ptr msg) {
            std::string messagePayload = msg->get_payload();
        }

        void onSocketOpen(websocketpp::connection_hdl hdl) {
            websocket::client::WebSocketClient::singletonClient.send(hdl,
                                                                     websocket::client::WebSocketClient::pendingSubscribeMessage,
                                                                     websocketpp::frame::opcode::text);
        }

        void initializeClient() {
            try {
                client* websocket = &websocket::client::WebSocketClient::singletonClient;
                // Set logging to be pretty verbose (everything except message payloads)
                websocket->set_access_channels(websocketpp::log::alevel::all);
                websocket->clear_access_channels(websocketpp::log::alevel::frame_payload);
                websocket->set_open_handler(&websocket::client::onSocketOpen);
                // Initialize ASIO
                websocket->init_asio();

                // Register our message handler
                websocket->set_message_handler(bind(
                        &websocket::client::onMessage, websocket, ::_1, ::_2)
                );

                websocketpp::lib::error_code ec;

                websocket::client::client::connection_ptr con = websocket->
                        get_connection(COINBASE_WEBSOCKET_URI, ec);
                if (ec) {
                    std::cout << "Could not create connection because: " << ec.message() << std::endl;
                    return;
                }

                // Note that connect here only requests a connection. No network messages are
                // exchanged until the event loop starts running in the next line.
                websocket->connect(con);

                // Start the ASIO io_service run loop
                // this will cause a single connection to be made to the server. c.run()
                // will exit when this connection is closed.

                websocket::client::WebSocketClient::connected = true;
                websocket->run();
            } catch (websocketpp::exception const & e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}


#endif //TRADINGSYSTEMS_WEBSOCKETCLIENT_H
