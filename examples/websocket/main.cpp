#include <iostream>
#include <cbpro++/auth.h>
#include <cbpro++/util/websocketclient.h>
#include <cbpro++/websocket/channel.h>

int main() {
    websocket_models::channel channel = websocket_models::channel(websocket_models::WebsocketChannels::heartbeat, { "BTC_USD", "ETH_USD" });

    WebSocketClient client = WebSocketClient();
    return 0;
}
