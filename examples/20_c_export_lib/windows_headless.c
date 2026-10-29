/* Non-UI COFF libraries provide the optional UI mailbox hooks explicitly. */
#include <stddef.h>
int xu_ui_host_run(const char *bootstrap) { (void)bootstrap; return 127; }
int xu_ui_mem_try_write(const char *path, const void *data, unsigned size, unsigned *written) {
    (void)path; (void)data; (void)size; if (written) *written = 0; return 0;
}
int xu_ui_mem_try_read_len(const char *path, unsigned *size) {
    (void)path; if (size) *size = 0; return 0;
}
int xu_ui_mem_try_read_copy(const char *path, void *out, unsigned cap) {
    (void)path; (void)out; (void)cap; return 0;
}
