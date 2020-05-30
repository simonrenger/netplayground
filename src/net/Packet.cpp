#include <net/Packet.h>

namespace eyos::net {
/////////////////////////////////
/// Packet  implementation   ///
///////////////////////////////

void Packet::Destroy()
{
    enet_free(enetPacket);
}

}
