#include <cstring>
#include <ctime>
#include <iostream>

#include "general.h"
#include "subscriber.h"

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    printf("%s Subscriber connected with code %d.\n", app::AppTag, rc);

    if (rc == 0)
    {
        subscribe(NULL, "my/awesome/topic");
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (!strcmp(message->topic, "my/awesome/topic"))
    {
        printf(
            "%s Got message with topic: %s, payload: %s\n",
            app::AppTag,
            message->topic,
            message->payload);
    }
}

void Subscriber::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    printf("%s Subscription succeeded.\n", app::AppTag);
}