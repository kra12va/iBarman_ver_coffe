

void make_cocktail(String cocktail){
  if(flag_drink_go == false){
  if(weight.container_flag){
    flag_drink_go = true;
    step = 1;
    weight.container = weight.curent;
    Serial.println("Масса стакана: " + String(weight.container));
    Serial.println("Получены параметры: ");
    Serial.println("Коктейль: " + cocktail);
    
    if(cocktail == "1"){ //
      Serial.println("Водка");
      recipe[0][0] = 1;
      recipe[0][1] = 50;

      recipe[1][0] = -10;
      recipe[1][1] = 0;      

    }
    if(cocktail == "2"){ //
      Serial.println("Газ вода");
      recipe[0][0] = 2;
      recipe[0][1] = 100;

      recipe[1][0] = -10;
      recipe[1][1] = 0;   
      
    }  
    if(cocktail == "3"){ //pinokolado
      Serial.println("Пиноколадо");
      recipe[0][0] = 1;
      recipe[0][1] = 50;

      recipe[1][0] = 3;
      recipe[1][1] = 30;      

      recipe[2][0] = 2;
      recipe[2][1] = 100;   

      recipe[3][0] = -10;
      recipe[3][1] = 0;      
      
    }    
    if(cocktail == "4"){ //mandarin
      Serial.println("Мандарин");
      recipe[0][0] = 1;
      recipe[0][1] = 50;

      recipe[1][0] = 4;
      recipe[1][1] = 30;      

      recipe[2][0] = 2;
      recipe[2][1] = 100;   

      recipe[3][0] = -10;
      recipe[3][1] = 0;      

    }
    if(cocktail == "5"){ //Вишня
      Serial.println("Вишня");
      recipe[0][0] = 1;
      recipe[0][1] = 50;

      recipe[1][0] = 5;
      recipe[1][1] = 30;      

      recipe[2][0] = 2;
      recipe[2][1] = 100;   

      recipe[3][0] = -10;
      recipe[3][1] = 0;      
      
    }
    if(cocktail == "6"){ //mohito
      Serial.println("Мохито");
      recipe[0][0] = 1;
      recipe[0][1] = 50;

      recipe[1][0] = 6;
      recipe[1][1] = 30;      

      recipe[2][0] = 2;
      recipe[2][1] = 100;   

      recipe[3][0] = -10;
      recipe[3][1] = 0;      
      
    }
    }else{
      Serial.println("Поставьте стакан");  
      flag_drink_go = false;
    }  

  }else{ 
    Serial.println("Коктейль еще наливается");
  }  
}




void listen_sensor(){

  if (listen_sensor_tmr.tick()) {
    
  };

}