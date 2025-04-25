#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  // Before exiting main, save your creation to disk as myImage.png
  //
  // alma
  // 900 X 600
  Image blanksheet(1000, 800);
  blanksheet.darken(1.0);
  Image cheering;
  cheering.readFromFile("../data/cheering.png");
  cheering.scale(0.5);
  std::cout << "CHEERING: " << cheering.width() << " X " << cheering.height() << std::endl;
  Image calm;
  calm.readFromFile("../data/calm.png");
  calm.scale(0.5);
  std::cout << "Calm: " << calm.width() << " X " << calm.height() << std::endl;

  Image pt_sti;
  pt_sti.readFromFile("../data/pt_sti.png");
  pt_sti.scale((0.85));
  std::cout << "Pointer: " << calm.width() << " X " << calm.height() << std::endl;

  Image vect_sti;
  vect_sti.readFromFile("../data/vect_sti.png");
  vect_sti.scale((0.85));
  std::cout << "Vector: " << calm.width() << " X " << calm.height() << std::endl;

  Image studentsTag; 
  studentsTag.readFromFile("../data/students.png");
  studentsTag.scale(0.75);


  StickerSheet meme(blanksheet);
  meme.addSticker(calm, 500, 0);
  meme.addSticker(cheering, 500, 400);
  meme.addSticker(pt_sti, 5, 50);
  meme.addSticker(vect_sti, 10, 450);
  meme.addSticker(studentsTag, 630, 370);


  Image render = meme.render();

  render.writeToFile("../data/myImage.png");

  std::cout << render.width() << std::endl;
  std::cout << render.height() << std::endl;

  // Image expected;
  // expected.readFromFile("../data/expected-6.png");

  return 0;
}
