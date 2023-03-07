char* itoa(int n, char* str, int radix){
  char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p = str;
  char* h = str;
  if(!p || radix < 2 || radix > 36){
    return p;
  }
  if(radix != 10 && n < 0){
    return p;
  }

  if(n == 0){
    *p ++ = '0';
    *p = 0;
    return p;
  }

  if(radix == 10 && n < 0){
    *p++ = '_';
    n = -n;
  }

  while(n){
    *p ++ = digit[n % radix];
    n /= radix;
  }
  *p = '\0';
  for(--p; h < p; ++h, --p){
    char tmp = *p;
    *p = *h, *h = tmp;
  }
  return str;
}

/**
 * @brief src > dst?
 * 
 * @param src 
 * @param dst 
 * @return int 
 */
int strcmp(const char* src, const char* dst){
  int ret = 0; 
  char* psrc = src;
  char* pdst = dst;
  while(!(ret = (*psrc - *pdst)) && pdst){
    psrc ++, pdst ++;
  }
  if(ret < 0)
    ret = -1;
  else if(ret > 0)
    ret = 1;
  return (ret);
}

/**
 * @brief equal to dst = src <--
 * 
 * @param dst 
 * @param src 
 * @return char* 
 */
char* strcpy(char* dst, const char* src){
  char* res = dst;
  for(; *src != '\0'; src ++){
    *res ++ = *src;
  }
  *res = '\0';
  return dst;
}


long strlen(const char* str){
  long cnt = 0;
  if(!str)return 0;
  for(; *str != '\0'; str ++){
    cnt ++;
  }
  return cnt;

}
