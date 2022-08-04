#include"implicits.hpp"

// this is how you declare an implicit
using a = implicits::val<int>;
using b = implicits::val<int>;

void implied_add(int& cnt){
    cnt += a::get();
}

int main(){
    int cnt{};
    //notice a and b are not the same type, if they did they would
    //reference the same value
    static_assert(!std::is_same_v<a, b>);
    
    //override a with 5
    a _(5);

    // no parameter passed, yet the fn can still access the 5 we just
    // put there
    implied_add(cnt);
    {
        a _(3);

        // now we call it with 3 as the implied argument
        implied_add(cnt);
    }
    // and now back to 5, since that 3 went away and brought back the 5
    implied_add(cnt);
    return cnt - 13; // two calls with 5 and one with 3 should yield 13
}
