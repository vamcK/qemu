#include "qemu/osdep.h"
#include "hw/virtio/virtio-nikhila.h"
#include "hw/virtio/virtio.h"
#include "hw/virtio/virtio-pci.h"
#include "qom/object_interfaces.h"

static void virtio_nic_set_status(VirtIODevice *vdev, uint8_t status)
{
    // VirtIONIC *vnic = VIRTIO_NIC(vdev);

    if (!vdev->vm_running) {
        return;
    }
    vdev->status = status;

    /* Something changed, try to process buffers */
    // virtio_rng_process(vrng);
}

static uint64_t get_features(VirtIODevice *vdev, uint64_t f, Error **errp)
{
    return f;
}

static void virtio_nic_device_realize(DeviceState *dev, Error **errp)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(dev);
    // VirtIONIC *vnic = VIRTIO_NIC(dev);
    // Error *local_err = NULL;

    virtio_init(vdev, "virtio-nic", VIRTIO_ID_NIKHILA, 0);

    // ADD VIRTQ LATER
    // vnic->vq = virtio_add_queue(vdev, 8, handle_input);

}

static void virtio_nic_device_unrealize(DeviceState *dev, Error **errp)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(dev);
    VirtIONIC *vnic = VIRTIO_NIC(dev);

    qemu_del_vm_change_state_handler(vnic->vmstate);
    virtio_cleanup(vdev);
}

static const VMStateDescription vmstate_virtio_nic = {
    .name = "virtio-nic",
    .minimum_version_id = 1,
    .version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_VIRTIO_DEVICE,
        VMSTATE_END_OF_LIST()
    },
};

static Property virtio_nic_properties[] = {};

static void virtio_nic_class_init(ObjectClass *klass, void *data)
{
    printf("nic.c : class init start \n");
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioDeviceClass *vdc = VIRTIO_DEVICE_CLASS(klass);

    dc->props = virtio_nic_properties;
    dc->vmsd = &vmstate_virtio_nic;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    vdc->realize = virtio_nic_device_realize;
    vdc->unrealize = virtio_nic_device_unrealize;
    vdc->get_features = get_features;
    vdc->set_status = virtio_nic_set_status;
    printf("nic.c : class init end \n");
}

static const TypeInfo virtio_nikhila_info = {
    .name = TYPE_VIRTIO_NIC,
    .parent = TYPE_VIRTIO_DEVICE,
    .instance_size = sizeof(VirtIONIC),
    .class_init = virtio_nic_class_init,
    // .abstract = true
};

static void virtio_register_types(void)
{
    printf("nic.c : register start \n");
    type_register_static(&virtio_nikhila_info);
    printf("nic.c : register end \n");
}

type_init(virtio_register_types)