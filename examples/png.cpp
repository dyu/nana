#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/picture.hpp>

struct Icon : nana::picture
{
    Icon(nana::widget& owner, const char* icon, bool cursor_hand = false) : nana::picture(owner)
    {
        load(nana::paint::image(icon));
        transparent(true);
        
        if (!cursor_hand)
            return;
        
        events().mouse_move([this](const nana::arg_mouse& arg) {
            cursor(nana::cursor::hand);
        });
    }
};

int main()
{
    using namespace nana;
    form    fm{ {273, 0, 1005, 710} };
    fm.bgcolor(colors::white);
    
    nana::place place{ fm };
    place.div("margin=5 <label_><up_ weight=16><down_ weight=16><todo_>");
    
    label   lb(fm, rectangle(fm.size()));
    lb.caption("Hello, world!");
    place["label_"] << lb;
    
    Icon up(fm, "examples/assets/arrow-up.png", true);
    place["up_"] << up;
    
    Icon down(fm, "examples/assets/arrow-down.png", true);
    place["down_"] << down;
    
    Icon icon(fm, "examples/assets/todo.png");
    place["todo_"] << icon;
    
    place.collocate();
    fm.show();
    exec();
}
