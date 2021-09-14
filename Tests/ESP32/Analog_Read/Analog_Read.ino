const int forceSensor1 = 34;
const int forceSensor2 = 35;
const int avarage_size = 200;

int val_fs1 = 0;
int val_fs2 = 0;

const float alpha = 0.05;
int moving_avarage_fs1 = 0;
int moving_avarage_fs2 = 0;
int moving_avarage_total = 0;

int classified = 0;


void setup() {
  Serial.begin(115200); 
}

void loop() {

  for(int i = 0; i < avarage_size; i++){
    val_fs1 = analogRead(forceSensor1);
    val_fs2 = analogRead(forceSensor2);

    moving_avarage_fs1 = (int) (alpha*val_fs1 + (1-alpha)*moving_avarage_fs1);
    moving_avarage_fs2 = (int) (alpha*val_fs2 + (1-alpha)*moving_avarage_fs2);

    delay(5);
  }

  moving_avarage_total = (int)(moving_avarage_fs1 + moving_avarage_fs2)/2;

  if(moving_avarage_total < 50){
    classified = 0;
  }else if(moving_avarage_total < 750){
    classified = 1;
  }else if(moving_avarage_total < 1400){
    classified = 2;
  }else if(moving_avarage_total < 2000){
    classified = 3;
  }else classified = 4;

  // Serial.print("Sensor 1: ");
  // Serial.print(val_fs1);
  // Serial.print("   Sensor 2: ");
  // Serial.print(val_fs2);
  Serial.print("Classified: ");
  Serial.print(classified);
  Serial.print("   Mov_Avg Total: ");
  Serial.print(moving_avarage_total);
  Serial.print("   Mov_Avg 1: ");
  Serial.print(moving_avarage_fs1);
  Serial.print("   Mov_Avg 2: ");
  Serial.println(moving_avarage_fs2);
}

