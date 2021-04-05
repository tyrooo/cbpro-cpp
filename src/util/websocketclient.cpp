//
// Created by Justin Barnett on 4/4/21.
//

#include "../../include/cbpro++/util/websocketclient.h"

namespace websocket {
    namespace client {

        bool WebSocketClient::subscribe(const std::vector<websocket::models::channel> channels,
                                        const std::vector <std::string> products) {
            pt::ptree subscribeMessageRoot, productIdsChild, channelsChild;

            subscribeMessageRoot.put("type", "subscribe");

            /**
             * All the products that we have
             */
            std::set<std::string> totalSet;
            for (websocket::models::channel wsModel: channels) {
                totalSet.merge(wsModel.getProductIds());
            }

            /**
             * The sets that are incommon for every product set
             */

            std::set<std::string> commonSet = std::set<std::string>(totalSet);
            for (websocket::models::channel wsModel: channels) {
                for (std::string productId: commonSet) {
                    if (wsModel.getProductIds().find(productId) ==
                        wsModel.getProductIds().end()) {
                        commonSet.erase(productId);
                    }
                }
            }

            for (std::string productId: commonSet) {
                productIdsChild.put("", productId);
            }
            subscribeMessageRoot.put_child("product_ids", productIdsChild);

            for (websocket::models::channel wsModel: channels) {
                std::set<std::string> workingSet = std::set<std::string>(wsModel.getProductIds());

                /**
                  * Remove the common items from the working set
                  */

                for (std::string productId: workingSet) {
                    if (commonSet.find(productId) != commonSet.end()) {
                        workingSet.erase(productId);
                    }
                }

                /**
                  * If the working set is 0 for this, then we should just get the channel name
                  * otherwise, get the entire JSON child
                  */

                if (workingSet.size() == 0) {
                    channelsChild.put("", wsModel.getChannelName());
                } else {
                    channelsChild.put_child("", wsModel.getPTree());
                }
            }

            subscribeMessageRoot.put_child("channels", channelsChild);
            websocket::client::WebSocketClient::pendingSubscribeMessage = toJsonString(subscribeMessageRoot);

            if (websocket::client::WebSocketClient::connected == false) {
                this->clientThread = new std::thread(&websocket::client::initializeClient);
            }
        }
    }
}
