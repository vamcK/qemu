#include "qemu/osdep.h"
#include "hw/virtio/virtio-nic.h"
#include "hw/virtio/virtio.h"
#include "hw/virtio/virtio-pci.h"
#include "qom/object_interfaces.h"
#include "hw/virtio/virtio-access.h"
#include "qemu/iov.h"

struct tosend {
   int a;
   char str[10];
};

struct torecieve {
   int a;
   char str[10];
};

static void handle_input(VirtIODevice *vdev, VirtQueue *vq)
{
    printf("-----virtio-nic.c: inside handle_input!-----\n");
    VirtQueueElement *elem;
    // size_t offset = 0, len;
    size_t offset = 0;
    struct tosend var;
    struct torecieve data;
    // size_t s;
    // char *str;
    // void *buf;
    int empty = virtio_queue_empty(vq);
    if(empty){
        printf("-----virtio-nic.c: empty queue\n");
    }
    elem = virtqueue_pop(vq, sizeof(VirtQueueElement));
    if (!elem) {
        printf("-----virtio-nic.c: no element popped\n");
        return;
    }
    printf("-----virtio-nic.c: element popped\n");
    printf("-----virtio-nic.c: element index:%d\n",elem->index);
    // virtqueue_drop_all(vq);
    // len = iov_to_buf(elem->out_sg, elem->out_num, offset, buf, 10);
    // str[len]='\0';
    // str=(char *)buf;
    // printf("----%s---%ld----\n",str, len);
    // offset+=len;
    // offset+=4;
    // uint16_t vid;
    size_t s;
    // s = iov_to_buf(elem->out_sg, elem->out_num, offset, &vid, sizeof(vid));
    // vid = virtio_lduw_p(vdev, &vid);
    // vid = lduw_be_p(&vid);
    // vid = lduw_le_p(&vid);
    // printf("\n---------\n");
    // s = writev(1, elem->out_sg, elem->out_num);
    // printf("\n---------%s\n",(char *)elem->out_sg[0].iov_base);
    // char data[10];
    // memcpy(data, elem->out_sg[0].iov_base, elem->out_sg[0].iov_len);
    // printf("-----data=%s\n",(char *)data);
    // s = iov_to_buf(elem->out_sg, elem->out_num, offset, data, sizeof(data));
    // printf("-----s=%ld\n",s);
    // offset+=s;
    // printf("-----virtio-nic.c: elem: outnum=%d\n",elem->out_num);
    // printf("-----virtio-nic.c: elem: innum=%d\n",elem->in_num);
    // struct iovec *iov;
    // unsigned int iov_cnt;
    // iov_cnt = elem->out_num;
    // iov = g_memdup(elem->out_sg, sizeof(struct iovec) * elem->out_num);
    // iov_to_buf(iov, iov_cnt, 0, data, sizeof(data));
    // printf("-----data=%s\n",(char *)data);
    // printf("-----%d\n",var.a);
    
    s = iov_to_buf(elem->out_sg, elem->out_num, offset, &var, sizeof(var));
    printf("-----var.a:%d\n",var.a);
    printf("-----var.str:%s\n",var.str);
    offset+=s;
    // iov_from_buf(const struct iovec *iov, unsigned int iov_cnt,
    //          size_t offset, const void *buf, size_t bytes)
    // data = (void *)elem->in_sg[0].iov_base;
    data.a=300;
    strcpy(data.str,"hello!");
    iov_from_buf(elem->in_sg, elem->in_num, offset, &data, sizeof(data));
    // &data->a=300;
    // virtio_stw_p(vdev, &data->a, 255);
    // printf("-----a:%d\n",&data->a);
    // strcpy(&data->str,"hello!");
    virtqueue_push(vq, elem, offset);
    virtio_notify(vdev, vq);
    g_free(elem);
}

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
    VirtIONIC *vnic = VIRTIO_NIC(dev);
    // Error *local_err = NULL;

    virtio_init(vdev, "virtio-nic", VIRTIO_ID_NIC, 0);

    // ADD VIRTQ LATER
    vnic->vq = virtio_add_queue(vdev, 8, handle_input);

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