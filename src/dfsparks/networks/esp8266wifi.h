#ifndef DF_ESP8266WIFI
#define DF_ESP8266WIFI (defined(ESP8266) || defined(ESP32))
#endif // DF_ESP8266WIFI

#if DF_ESP8266WIFI
#ifndef DFSPARKS_NETWORK_ESP8266_H
#define DFSPARKS_NETWORK_ESP8266_H
#include "dfsparks/network.h"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

namespace dfsparks {

class Esp8266Network : public Network {
public:
  Esp8266Network(const char* ssid, const char* passwd);

private:
  void doConnection() final;
  int doReceive(void *buf, size_t bufsize) final;
  int doBroadcast(void *buf, size_t bufsize) final;

  struct Credentials {
    const char* ssid;
    const char* pass;
  };

  WiFiUDP udp_;
  Credentials creds_;
  bool needToBegin_ = true;
};

} // namespace dfsparks

#endif // DFSPARKS_NETWORK_ESP8266UDP_H
#endif // DF_ESP8266WIFI
