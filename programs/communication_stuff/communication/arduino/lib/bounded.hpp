#ifndef BOUNDED_HPP
#define BOUNDED_HPP
#include <Arduino.h> // Size_t

#define FILL_BYTE 0xff

template <size_t S, typename T> // S must be power of 2
struct bounded_counter
{
private:
  // Return true if S is the length of T
  // The type is already range-limited
  bool isTsize(){
    size_t max = T(-1) + 1;
    return S == max;
  }

public:
  T c = 0; // Counter

  T &operator+=(const T &other)
  {
    /*
    if(c > (S-1 - other)){
      c+=other;
      c &= S-1;
      return c;
    }
    c+=other;
    */
    c+=other;
    if(!isTsize()) 
      c &= S-1;
    return c;
  }

  T operator++(int){
    T tmp = c;
    if(isTsize())
      c++;
    else{
      if(c == (S-1))
        c = 0;
      else
        c+=1;
    }
    return tmp;
  }

  T &operator-=(const T &other){
    c-=other;
    if(!isTsize())
      c &= S-1;
    return c;
  }

  T operator--(int){
    T tmp = c;
    if(isTsize())
      c--;
    else{
      if(c == 0)
        c = S-1;
      else
        c-=1;
    }
    return tmp;
  }
  
  
  T &operator= (const T &other){
    if(isTsize())
      c = (T) other;
    else
      c = (other & (S-1));
    return c;
  }

  friend T operator+(bounded_counter<S, T> lhs, const T &rhs){
    lhs += rhs;
    return lhs.c;
  }
  
  friend T operator-(bounded_counter<S, T> lhs, const T &rhs){
    lhs -= rhs;
    return lhs.c;
  }

  operator T() const { return c; }

  // behave like a normal integer
  inline bool operator==(const T& other){ return c == other; }
  inline bool operator< (const T& other){ return c <  other; }
  inline bool operator> (const T& other){ return c >  other; }
  inline bool operator<=(const T& other){ return c <= other; }
  inline bool operator>=(const T& other){ return c >= other; }
  inline bool operator!=(const T& other){ return c != other; }

  bounded_counter(){
    static_assert( !(S&(S-1)), "Size must be power of 2");
    static_assert( T(-1) > T(0), "Type must be unsigned!" );
  }
};

template <size_t S, typename T> // S must be power of 2, T should be unsigned
struct bounded_buf
{
  uint8_t buf[S];
  bounded_counter<S,T> wpos; // Write pos
  bounded_counter<S,T> rpos; // Read pos

  /* Write NUM bytes from ARR to buf starting from position POS */
  void writeArray(uint8_t *arr, T num, T pos){
    if( num > S - pos ){
      T start = pos;
      T first = S - pos;
      memcpy(buf + pos, arr, first);
      T second = num - first;
      memcpy(buf, arr+first, second);
    }else
      memcpy(buf + pos, arr, num);
    // Set write pos
    wpos = (pos + num) & (S-1);
  }

  void readArray(uint8_t *arr, T num, T pos){
    if( num > S - pos ){
      T start = pos;
      T first = S - pos;
      memcpy(arr, buf + pos, first);
      memset(buf + pos, FILL_BYTE, first);
      T second = num - first;
      memcpy(arr+first, buf, second);
      memset(buf, FILL_BYTE, second);
    }else
      memcpy(arr, buf + pos, num);
      memset(buf+pos, FILL_BYTE, num);

    // Set write pos
    rpos = (pos + num) & (S-1);
  }

  void readArray(uint8_t *arr, T num){
    readArray(arr, num, rpos.c);
  }

  uint8_t &operator[] (size_t index){
    index &= S-1;
    return buf[index];
  }

  uint8_t &operator[] (bounded_counter<S, T> count){
    return buf[count.c];
  }

  uint8_t readByte(){
    //rpos &= S-1;
    T index = rpos++;
    uint8_t res = buf[index];
    buf[index] = FILL_BYTE; // Clear buffer after reading
    return res;
  }

  T writeByte(uint8_t b){
    //wpos &= S-1;
    buf[wpos.c] = b;
    wpos++;
    return wpos.c;
  }

  /* Stuff for recieve buffer */
  bool isRecieved(){
    uint8_t len = buf[rpos.c];
    if(len == FILL_BYTE)
      return false; // No instruction

    uint8_t start_pos = 0;
    uint8_t end_pos = len;
    // [Start_pos](0) --------------------- [End_pos](len) ------------------ [End](S-1)
    // writer_pos:           Invalid                              Valid
    uint8_t writer_pos = wpos - rpos.c; // Bounded
    if(writer_pos < end_pos)
      return false;
    if(end_pos < writer_pos)
      return true;
    
    //perror("Cannot decide if whole message recieved\n");
    //Serial.println("Cannot decide if whole message recieved\n");
    //abort();
    //delay(1000*5);
    return false; // Prevent compiler from complaining
  }
};

#endif