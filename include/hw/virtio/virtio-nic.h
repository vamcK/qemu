#include "hw/virtio/virtio.h"
#include "standard-headers/linux/virtio_rng.h"


#define TYPE_VIRTIO_NIC "virtio-nic-device"
#define VIRTIO_NIC(obj) \
        OBJECT_CHECK(VirtIONIC, (obj), TYPE_VIRTIO_NIC)
#define VIRTIO_NIC_GET_PARENT_CLASS(obj) \
        OBJECT_GET_PARENT_CLASS(obj, TYPE_VIRTIO_NIC)
#include "standard-headers/linux/types.h"
#include "standard-headers/linux/virtio_types.h"
#include "standard-headers/linux/virtio_ids.h"
#include "standard-headers/linux/virtio_config.h"

typedef struct VirtIONIC {
    VirtIODevice parent_obj;
    // VirtQueue *vq;

    // RngBackend *rng;

    // /* We purposefully don't migrate this state.  The quota will reset on the
    //  * destination as a result.  Rate limiting is host state, not guest state.
    //  */
    // QEMUTimer *rate_limit_timer;
    // int64_t quota_remaining;
    // bool activate_timer;

    VMChangeStateEntry *vmstate;
} VirtIONIC;