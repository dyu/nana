#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/label.hpp> 
#include <nana/gui/widgets/button.hpp> 

int main() 
{ 
    nana::form fm({0, 0, 400, 300});
    fm.bgcolor(nana::colors::white);
    fm.caption(("Root"));
    fm.events().unload([](const nana::arg_unload& arg){
        nana::API::exit();
    });
    
    nana::form fm2(fm, {0, 0, 400, 280}, nana::appearance(
        true,//0 != (flags & WindowFlags::DECORATION),
        false,//0 != (flags & WindowFlags::TASKBAR),
        false,//0 != (flags & WindowFlags::FLOATING),
        false,//0 != (flags & WindowFlags::NO_ACTIVATE),
        false,//0 != (flags & WindowFlags::MINIMIZE),
        false,//0 != (flags & WindowFlags::MAXIMIZE),
        false//0 != (flags & WindowFlags::SIZABLE)
    ));
    fm2.bgcolor(nana::colors::white);
    fm2.caption("Child");
    fm2.events().unload([&fm, &fm2](const nana::arg_unload& arg) {
        arg.cancel = true;
        fm2.hide();
        //nana::API::window_enabled(fm, true);
    });
    
    auto $btn_clicked = [&fm, &fm2](const nana::arg_click& arg) {
        if (fm2.visible())
        {
            fm2.hide();
        }
        else
        {
            auto pos = nana::API::window_position(fm);
            pos.y += 20;
            nana::API::move_window(fm2, pos);
            
            fm2.show();
            //nana::API::window_enabled(fm, false);
        }
    };
    
    auto $listener = [&fm, &fm2](nana::label::command cmd, const std::string& target) {
        if (nana::label::command::click != cmd)
            return;
        
        if (fm2.visible())
        {
            fm2.hide();
        }
        else
        {
            auto pos = nana::API::window_position(fm);
            pos.y += 20;
            nana::API::move_window(fm2, pos);
            
            fm2.show();
            //nana::API::window_enabled(fm, false);
        }
    };
    
    nana::place plc(fm);
    plc.div("vert<lbl_><btn_>");
    
    nana::label lbl(fm);
    plc["lbl_"] << lbl
        .text_align(nana::align::center)
        .format(true)
        .add_format_listener($listener)
        .caption("<target=\"foo\">bar</>");
    
    nana::button btn(fm, "test");
    btn.fgcolor(nana::colors::white);
    btn.bgcolor(nana::color_rgb(0x0080FF));
    btn.borderless(true);
    btn.events().click($btn_clicked);
    plc["btn_"] << btn;
    
    plc.collocate();
    fm.show(); 
    nana::exec(); 
} 
