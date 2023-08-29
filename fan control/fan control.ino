#define Gate 3
void setup () {
pinMode (Gate, OUTPUT);
}
void loop () {
analogWrite(Gate, 0);
delay(10000);
analogWrite(Gate, 0);
delay(5000);
}