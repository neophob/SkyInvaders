//parse json object

void parseJson(char *jsonString) {
  aJsonObject* root = aJson.parse(jsonString);  
  
  //Now we can e.g. retrieve the value for name:  
  aJsonObject* name = aJson.getObjectItem(root, "name");

  //The value of name can be retrieved via:
  Serial.println(name->valuestring);

}
