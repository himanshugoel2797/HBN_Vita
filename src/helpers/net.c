#include "helpers/net.h"

#include <stdlib.h>
#include <string.h>

#include <psp2/sysmodule.h>
#include <psp2/net/netctl.h>
#include <psp2/net/net.h>
#include <psp2/libssl.h>
#include <psp2/net/http.h>
#include <psp2/io/fcntl.h>

#define NET_INIT_SIZE (64 * 1024)
#define DL_BUFFER_SIZE 4096
#define USERAGENT_STR "PSV-HBN_App_100"

static int err_code = 0;
static SceNetCtlInfo net_info;

void
InitializeNet(void) {

    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
	err_code = sceNetShowNetstat();
	if (err_code == SCE_NET_ERROR_ENOTINIT) {
		SceNetInitParam initparam;
		initparam.memory = malloc(NET_INIT_SIZE);
		initparam.size = NET_INIT_SIZE;
		initparam.flags = 0;
		err_code = sceNetInit(&initparam);
	}
	
    if(err_code < 0)return;

    err_code = sceNetCtlInit();
    if(err_code < 0)return;

	err_code = sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_ADDRESS, &net_info);
    if(err_code < 0)return;

    err_code = sceSslInit(NET_INIT_SIZE);
    if(err_code < 0)return;

    err_code = sceHttpInit(NET_INIT_SIZE);
    if(err_code < 0)return;

	SceNetInAddr vita_addr;
	sceNetInetPton(SCE_NET_AF_INET, net_info.ip_address, &vita_addr);


}

int
NetIsInitialized(void){
    return err_code;
}

void
GetCurrentIP(char ip[]) {
    strcpy(ip, net_info.ip_address);
}

void
GetDHCPName(char dst[256]) {
    strcpy(dst, net_info.dhcp_hostname);
}

int 
DownloadFile(const char *remote, const char *loc){
    int template = sceHttpCreateTemplate(USERAGENT_STR, SCE_HTTP_VERSION_1_1, SCE_HTTP_PROXY_AUTO);
    if(template < 0)
        return template;

    int conn = sceHttpCreateConnectionWithURL(template, remote, 0);
    if(conn < 0)
        return conn;

    int req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, remote, 0);
    if(req < 0)
        return req;

    int ret = sceHttpSendRequest(req, NULL, 0);
    if(ret < 0)
        return ret;
 
    uint8_t *buffer = malloc(DL_BUFFER_SIZE);
    if(buffer == NULL)
        return -1;

    SceUID fd = sceIoOpen(loc, SCE_O_WRONLY | SCE_O_CREAT, 0777);
    if((int)fd < 0)
        return -2;

    while((ret = sceHttpReadData(req, buffer, DL_BUFFER_SIZE)) > 0)
        sceIoWrite(fd, buffer, ret);
    sceIoClose(fd);

    sceHttpDeleteRequest(req);
    sceHttpDeleteConnection(conn);
    sceHttpDeleteTemplate(template);

    return ret;
}