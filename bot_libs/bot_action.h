class bot_action {
  public:
    bot_action(int pin[], int pin_len) {
      _pin_len = pin_len;
      for (int i = 0; i < pin_len; i++) {
        _pin[i+1] = pin[i];
        pinMode(_pin[i+1], OUTPUT);
      }
    }

    void print_pins() {
      for (int i = 1; i <= _pin_len; i++) {
       Serial.println("Пин: " + String(i) + " - " + String(_pin[i])); 
      }      
    }
    void start_pure_drink(int pin){
      digitalWrite(pin, 1);
    }
    void stop_pure_drink(int pin){
      digitalWrite(pin, LOW);
    }   

    int drink_number(int i){
      return _pin[i];
    }


  private:
    int _pin_len;
    int _pin[12];
    //uint32_t _tmr;
    //uint16_t _prd;
    bool _flag_start = true; 
};