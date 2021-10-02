#ifndef _ATCOMMAND_H_
#define _ATCOMMAND_H_

#define notCorrectCommand -1
#define notCorrectAT -2
#define readTime 1
#define setTime 2

#define scanWifiAP 3
#define checkWiFi 4
#define connWiFi 5
#define connWiFiWithPswd 6
#define disconnectWiFi 7

int getATCommand();

#endif
