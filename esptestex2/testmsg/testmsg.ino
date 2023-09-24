void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("testVALVE");
  delay(10);
  Serial.println("testmsg");
  delay(10);
  Serial.println("testvalve");
  delay(10);
  Serial.println("test");
  delay(10);
  Serial.println("Valve");
  delay(10);
  Serial.println("\xe2\x86\x90[0m bytetest2");
  delay(10);
  Serial.println("\xe2\x86\x90\xc2\x86\xc2\x90[0;32m bytetest");
  delay(10);
  Serial.println("SUUUUPERLONGSTRING1234567890qwerty0987654321{texttexttext}[0abcd0](samcraftsam)======}}}}zxczxczxc");
  delay(10);
  Serial.print("QWERTY LONG TEXT 2");
  Serial.print("ALPHA beta [xx0d000]()()()");
  Serial.print("1234567890987654321");
  Serial.print("[xx0d000]()()()");
  delay(50);
  Serial.println("");
}
