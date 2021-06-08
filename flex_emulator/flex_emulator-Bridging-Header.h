//
//  Use this file to import your target's public headers
//  that you would like to expose to Swift.
//

#ifndef bridge_h
#define bridge_h

#ifdef __cplusplus
extern "C" {
#endif

const void *something_construct();
void something_destruct(const void *object);
int something_add(const void *object, int a, int b);

#ifdef __cplusplus
}
#endif
#endif /* bridge_h */
