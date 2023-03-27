// отправить на индикаторы
void sendTime(byte hours, byte minutes, byte secs) {
  indiDigits[0] = (byte)hours / 10;
  indiDigits[1] = (byte)hours % 10;

  indiDigits[2] = (byte)minutes / 10;
  indiDigits[3] = (byte)minutes % 10;

  indiDigits[4] = (byte)secs / 10;
  indiDigits[5] = (byte)secs % 10;

  indiDigits[6] = (byte)(60-secs) / 10;
  indiDigits[7] = (byte)(60-secs) % 10;

  //indiDigits[4] = (byte)date / 10;
  //indiDigits[5] = (byte)date % 10;

  //indiDigits[6] = (byte)month / 10;
  //indiDigits[7] = (byte)month % 10;
  /*
  indiDigits[4] = year / 1000;
  indiDigits[5] = (year / 100) % 10;
  indiDigits[6] = (year % 100) / 10;
  indiDigits[7] = year % 10;
*/
}

// для эффектов
void setNewTime() {
  newTime[0] = (byte)hrs / 10;
  newTime[1] = (byte)hrs % 10;

  newTime[2] = (byte)mins / 10;
  newTime[3] = (byte)mins % 10;

  newTime[4] = (byte)secs / 10;
  newTime[5] = (byte)secs % 10;

  newTime[6] = (byte)(60-secs) / 10;
  newTime[7] = (byte)(60-secs) % 10;

  //newTime[4] = (byte)date / 10;
  //newTime[5] = (byte)date % 10;

  //newTime[6] = (byte)month / 10;
  //newTime[7] = (byte)month % 10;
}
