//
// Created by Justin Barnett on 4/4/21.
//

#ifndef TRADINGSYSTEMS_WEBSOCKET_MODELS_H
#define TRADINGSYSTEMS_WEBSOCKET_MODELS_H

#include <boost/assign/list_of.hpp>
#include <unordered_map>
#include <vector>
#include <set>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;
using boost::assign::map_list_of;

namespace websocket {
    namespace models {
        typedef enum WebsocketChannels {
            heartbeat, status, ticker, level2, user, matches, full
        } e_WebsocketChannels;

        class channel {
        public:
            channel(e_WebsocketChannels channel, std::set<std::string> productIds);

            /**
             * Get the channel type of this websocket channel
             * @return
             */
            [[nodiscard]] e_WebsocketChannels getChannelType() const;

            /**
             * Create the JSON for this websocket Channel
             * @return
             */
            [[nodiscard]] pt::ptree getPTree() const;
            [[nodiscard]] std::string getChannelName() const;
            [[nodiscard]] std::set<std::string> getProductIds() const;
        private:
            e_WebsocketChannels channelType;
            std::set<std::string> productIds;
        };

    }
}

#endif //TRADINGSYSTEMS_WEBSOCKET_MODELS_H
