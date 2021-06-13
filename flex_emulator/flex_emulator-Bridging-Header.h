//
//  Use this file to import your target's public headers
//  that you would like to expose to Swift.
//

#ifndef bridge_h
#define bridge_h

#ifdef __cplusplus
extern "C" {
#endif

const void *machine_construct();
void machine_destruct(const void *object);
void machine_step(const void *object);
void machine_queue_key_press(const void *object, char key);
int machine_dequeue_serial_output(const void *object);
long long machine_cycles_spent(const void *object);

unsigned char *get_saa5050_font(void);

#ifdef __cplusplus
}
#endif
#endif /* bridge_h */
