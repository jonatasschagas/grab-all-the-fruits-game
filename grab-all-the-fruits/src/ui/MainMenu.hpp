#pragma once
#ifndef MainMenu_hpp 
#define MainMenu_hpp

#include "ButtonClickListener.hpp"

class MainMenu
{
public:

    MainMenu(const int screenWidth, const int screenHeight);
    ~MainMenu();
    
    void update();

    void setButtonClickListener(ButtonClickListener* pButtonClickListener);

private:

    ButtonClickListener* m_pButtonClickListener;
    bool m_showCredits;
    int m_screenWidth;
    int m_screenHeight;

    void initializeMembers()
    {
        m_pButtonClickListener = nullptr;
        m_showCredits = false;
        m_screenWidth = 0;
        m_screenHeight = 0;
    }

};

#endif /* MainMenu_hpp */
