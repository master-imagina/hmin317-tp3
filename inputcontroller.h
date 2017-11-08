#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H


class InputController
{
public:
    InputController();
    bool getKeyZ() const;
    bool getKeyS() const;
    bool getKeyQ() const;
    bool getKeyD() const;
    bool getKeyShift() const;
    bool getKeySpace() const;
    bool getMouseLeftButton() const;

    void setKeyZ(bool );
    void setKeyS(bool );
    void setKeyQ(bool );
    void setKeyD(bool );
    void setMouseLeftButton(bool );

    void keyPressed(int);

    void keyReleased(int);

    void mousePressed(int);
    void mouseReleased(int);

private:
    bool KEY_Z = 0;
    bool KEY_S = 0;
    bool KEY_Q = 0;
    bool KEY_D = 0;
    bool KEY_SHIFT = 0;
    bool KEY_SPACE = 0;
    bool MOUSE_LEFTBUTTON = 0;
};

#endif // INPUTCONTROLLER_H
