//
// Created by Justin Barnett on 4/4/21.
//

#include <boost/property_tree/json_parser.hpp>
#include "../../include/cbpro++/websocket/channel.h"


namespace websocket {
    namespace models {
        std::string WebsocketMapToString(e_WebsocketChannels channel) {
            switch (channel) {
                case e_WebsocketChannels::heartbeat:
                    return "heartbeat";
                case e_WebsocketChannels::status:
                    return "status";
                case e_WebsocketChannels::ticker:
                    return "ticker";
                case e_WebsocketChannels::level2:
                    return "level2";
                case e_WebsocketChannels::user:
                    return "user";
                case e_WebsocketChannels::matches:
                    return "matches";
                case e_WebsocketChannels::full:
                    return "full";
                default:
                    return "";
            }
        }

        channel::channel(e_WebsocketChannels channel, std::set<std::string> productIds) {
            this->channelType = channel;
            this->productIds = productIds;
        }

        pt::ptree channel::getPTree() const {
            pt::ptree root, productIds;
            root.put("name", websocket::models::WebsocketMapToString(this->channelType));

            for (std::string productId: this->productIds) {
                productIds.put("", productId);
            }
            root.add_child("product_ids", productIds);

            return root;
        }

        std::string channel::getChannelName() const {
            return websocket::models::WebsocketMapToString(this->channelType);
        }

        std::set<std::string> channel::getProductIds() const {
            return this->productIds;
        }
    }
}