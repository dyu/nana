/**
 *  \file calculator.cpp
 *  \brief Nana Calculator
 *  Nana 1.3 and C++11 is required.
 *  This is a demo for Nana C++ Library.
 *  It creates an intermediate level graphical calculator with few code.
 */

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <forward_list>
#include <map>
#include <cassert>

//#include <nana/paint/graphics.hpp"

#include <iostream>
#include <chrono>

#include <thread>

// workaround insufficiency in VS2013.
#if defined(_MSC_VER) && (_MSC_VER < 1900)  //VS2013
const std::string plus_minus(to_utf8(L"\u00b1")  ;   // 0xB1    u8"\261"
#else
const std::string plus_minus( u8"\u00b1" );
#endif

struct StateInfo
{
    enum class state{init, operated, assigned};

    state          opstate{ state::init };
    std::string    operation;
    double         oprand { 0 };
    double         outcome{ 0 };
    nana::label&   procedure;
    nana::label&   result;
    
    StateInfo(nana::label& proc, nana::label& resl)
        : operation("+"),  procedure(proc), result(resl)
    {   }
};

void numkey_pressed(StateInfo& state, const nana::arg_click& arg)
{
    if (state.opstate != StateInfo::state::init)
    {
        if (state.opstate == StateInfo::state::assigned)
        {
            state.outcome = 0;
            state.operation = "+";          
        }
        state.result.caption("");
        state.opstate = StateInfo::state::init;
    }
    
    std::string rstr = state.result.caption();
    if (rstr == "0")
        rstr.clear();
    
    std::string d = nana::API::window_caption(arg.window_handle);
    if ("." != d)
        state.result.caption(rstr + d);
    else if (rstr.npos == rstr.find('.'))
        state.result.caption(rstr.size() ? rstr + d : std::string("0."));
}

void opkey_pressed(StateInfo& state, const nana::arg_click& arg)
{
    std::string d = nana::API::window_caption(arg.window_handle) ;
    if ("C" == d)
    {
        state.result.caption("0");
        state.procedure.caption("");
        state.opstate = StateInfo::state::init;
        state.outcome = 0;
        state.operation = "+";
        return;
    }
    else if (plus_minus == d)
    {
        auto s = state.result.caption();
        if (s.size())
        {
            if (s[0] == '-')
                s.erase(0, 1);
            else
                s.insert(0, 1, '-');

            if (state.opstate == StateInfo::state::assigned)
            {
                state.outcome = -state.outcome;
                state.operation = "=";
            }

            state.result.caption(s);
            state.opstate = StateInfo::state::init;
        }
        return;
    }
    else if ("%" == d)
    {
        auto s = state.result.caption();
        if (s.size())
        {
            double d = std::stod(s);
            d = state.outcome * d / 100;
            state.result.caption(std::to_string(d));
            state.opstate = StateInfo::state::init;
        }
        return;         
    }
    else if (state.opstate == StateInfo::state::operated)
        return;
    
    std::string oprandstr = state.result.caption();
    if (0 == oprandstr.size())
        oprandstr = '0';
    
    std::string pre_operation = state.operation;
    std::string proc;
    if ("=" != d)
    {
        state.operation = d;
        if (state.opstate != StateInfo::state::assigned)
            state.oprand = std::stod(oprandstr);
        else
            pre_operation = "=";
        
        proc =  state.procedure.caption()  + oprandstr ;
        if (("X" == d || "/" == d) && (proc.find_last_of("+-") != proc.npos))
        {
            proc.insert(0, "(");
            (( proc += ") " )  += d) += " ";
        }
        else
        {
            ((proc += " ") += d) += " ";
        }
        
        state.opstate = StateInfo::state::operated;
    }
    else
    {
        if (state.opstate == StateInfo::state::init)
            state.oprand = std::stod(oprandstr);
        
        state.opstate = StateInfo::state::assigned;
    }
    
    switch (pre_operation[0])
    {
        case '+': state.outcome += state.oprand; break;
        case '-': state.outcome -= state.oprand; break;
        case 'X': state.outcome *= state.oprand; break;
        case '/': state.outcome /= state.oprand; break;
    }
    
    state.procedure.caption(proc);
    
    std::string outstr = std::to_string(state.outcome);
    while (outstr.size() && ('0' == outstr.back()))
        outstr.pop_back();
    
    if (outstr.size() && (outstr.back() == '.'))
        outstr.pop_back();
    
    if (outstr.empty())
        outstr += '0';
    
    state.result.caption(outstr);
}

static const char LAYOUT[] =
"vert<procedure weight=12% margin=[0,4,0]><result weight=12% margin=[0,8,0]>"
"<weight=2><opkeys margin=2 grid=[4,5] gap=2 collapse(0,4,2,1)>"
;

int main()
{
    nana::form fm;
    fm.caption(("Calculator"));
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    //Use class place to LAYOUT the widgets.
    nana::place place(fm);
    place.div(LAYOUT);
    
    nana::label procedure(fm), result(fm);
    
    //procedure.bgcolor(color_rgb(0xFFFFFF));
    //result.bgcolor(color_rgb(0xFFFFFF));

    //Make the label right aligned.
    procedure.text_align(nana::align::right);
    result.text_align(nana::align::right);
    result.typeface(nana::paint::font("", 14, true));
    
    place["procedure"] << procedure;
    place["result"] << result;
    
    StateInfo state(procedure, result);
    
    std::forward_list<nana::button> btn_list;
    std::map<char,nana::button*> btn_map;
    
    char keys[] = "Cm%/789X456-123+0.="; // \261
    nana::paint::font keyfont("", 10, true);
    
    for (auto key : keys)
    {
        std::string caption;
        if (key == 'm')
            caption = plus_minus;  
        else
            caption = std::string(1, key);
        
        btn_list.emplace_front(fm.handle());
        
        auto& key_btn = btn_list.front();
        
        btn_map[key] = &key_btn;
        
        key_btn.caption(caption);
        key_btn.typeface(keyfont);
        
        if ('=' == key)
        {
            key_btn.bgcolor(nana::color_rgb(0x0080FF));
            key_btn.fgcolor(nana::color_rgb(0xFFFFFF));
        }
        else
        {
            key_btn.bgcolor(nana::color_rgb(0xFFFFFF));
        }
        
        place["opkeys"] << key_btn;
        
        //Make event answer for keys.
        key_btn.events().click([key, &state](const nana::arg_click& arg)
        {
            if (('0' <= key && key <= '9') || ('.' == key))
                numkey_pressed(state, arg);
            else
                opkey_pressed(state, arg);
        });
    }
    
    place.collocate();
    fm.show();
    nana::exec();
}
