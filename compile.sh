gcc -shared -fPIC -o libhook.so libhook.c -ldl
gcc -o manage_hw_bp manage_hw_bp.c
