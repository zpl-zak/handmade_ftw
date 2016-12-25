  #ifdef __cplusplus
  extern "C"
  {
#endif
    #pragma function(memset)
    void *memset(void *dest, int c, size_t count)
    {
        char *bytes = (char *)dest;
        while (count--)
        {
            *bytes++ = (char)c;
        }
        return dest;
    }
#ifdef __cplusplus
}
#endif