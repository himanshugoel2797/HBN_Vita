#ifndef PSV2CTRL_NET_H
#define PSV2CTRL_NET_H

#include <stdint.h>

void
InitializeNet(void);

int
NetIsInitialized(void);

void
GetCurrentIP(char ip[16]);

void
GetDHCPName(char dst[256]);

int 
DownloadFile(const char *remote, const char *loc);

#endif