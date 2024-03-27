//изменение энкодеров
void enc_A() {
  motorA.writeEnc();
}
void enc_B() {
  motorB.writeEnc();
}
void enc_C() {
  motorC.writeEnc();
}
void enc_D() {
  motorD.writeEnc();
}

void clearAllEnc() {
  motorA.clear_enc();
  motorB.clear_enc();
  motorC.clear_enc();
  motorD.clear_enc();
}

//  motorA.followSpeed(50); //мотор вызывается вот так
