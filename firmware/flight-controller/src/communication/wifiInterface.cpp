#include "wifiInterface.h"


bool WiFiInterface::isConnected(){
    connected = bool(WiFi.status() == WL_CONNECTED);
    return connected;
}

bool WiFiInterface::begin(const char* ssid, const char* password){ 
    //try connecting to WiFi-network
    WiFi.begin(ssid, password);

    //await conenction for at most MAX_WAITING_TIME
    int tries = 0;
    int max_tries = int(MAX_WAITING_TIME/500);
    while(!isConnected()){
        delay(500);
        if (tries>=max_tries){
            //timeout: connection failed
            WiFi.disconnect();
            connected=false;
            return false;
        }
        tries++;
    }

    //succesfully connected to network
    //Try to setup Udp socket
    connected = Udp.begin(LOCAL_UDP_PORT);

    return connected;
}

bool WiFiInterface::send(const uint8_t* message, size_t size){
    //check for connection
    if(!isConnected()){
        return false;
    }

    //failed to begin packet
    if(!Udp.beginPacket(pcIP, pcPort)){
        return false;
    }

    //failed to write message to buffer
    if(!Udp.write(message, size)){
        return false;
    }

    //failed to send packet
    if(!Udp.endPacket()){
        return false;
    }
    
    //all succesful
    return true;
}

size_t WiFiInterface::receive(uint8_t* message, size_t size){
    //get size of next available packet
    size_t packetSize = Udp.parsePacket();

    //no packet available -> return 0
    if (!packetSize){
        return 0;
    }

    //get IP and port from sender -> used to send packets later
    pcIP = Udp.remoteIP();
    pcPort = Udp.remotePort();

    //read into buffer and return amout of bytes received
    size_t bytesReceived = Udp.read(message, size);
    return bytesReceived;
}