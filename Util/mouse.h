#ifndef MOUSE_H
#define MOUSE_H

class Mouse {
  private:
	static float x_pos;
	static float y_pos;

  public:
	Mouse() = delete;
	static void update(struct GLFWwindow *window);
	static float x();
	static float y();
};

#endif // MOUSE_H
