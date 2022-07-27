//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_PLAYERCOLOUR_H_DATE_27_07_2022_TIME_10_03
#define PENTAGO_PLAYERCOLOUR_H_DATE_27_07_2022_TIME_10_03


class PlayerColour {
public:
    static PlayerColour White() { return PlayerColour(1); }
    static PlayerColour Black() { return PlayerColour(2); }

    friend std::ostream& operator<<(std::ostream& os, const PlayerColour& obj);

protected:
    explicit PlayerColour(short v) : val(v) {}
    const short val;

    inline bool operator==(const PlayerColour& rhs) const { return val == rhs.val; }
    inline bool operator!=(const PlayerColour& rhs) const { return !(*this == rhs); }
};

std::ostream& operator<<(std::ostream& os, const PlayerColour& obj) {
    switch (obj.val) {
        case 1: os << 'W';
            break;
        case 2: os << 'B';
            break;
    }
    return os;
}


#endif //PENTAGO_PLAYERCOLOUR_H_DATE_27_07_2022_TIME_10_03
