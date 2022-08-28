#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

libusb_device **list;
libusb_device_handle *handle;
struct libusb_device_descriptor desc;
unsigned char buffer[512];
unsigned char buffer2[512];
int cnt, i;

void get_text ();

int main() {
  libusb_init(NULL);
  cnt = libusb_get_device_list(NULL, &list);
  
  for (i = 0; i < cnt; i++) {
    libusb_device *device = list[i];
    libusb_get_device_descriptor(device, &desc);
    if (libusb_open(device, &handle) == 0) {
      
      libusb_get_string_descriptor_ascii(handle, desc.iProduct, buffer, sizeof(buffer));
        
      if (strcmp("Template", (char const *)buffer) == 0) {
        while (1) {
          get_text();
          usleep(100000);
        }
        break;
      }

      libusb_close(handle);
    }
  }

  libusb_free_device_list(list, 1);
  libusb_exit(NULL);  
  return 0;
}

void get_text () {
  int b;
  b = libusb_control_transfer(handle,
    LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
    0, 0, 0, buffer2, sizeof(buffer2), 5000);
  if (b > 0) {
    printf("☃\n");
  }
}
