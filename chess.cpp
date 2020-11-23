#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;
using bboard = unsigned long long;


void getInitialBoardState(bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&, bboard&);
void printBoardState(bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard);
void getBBoard(bboard&, string);
void printBBoard(bboard);

bboard getWhiteMoves(bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard, bboard);
string getWhitePawnMoves(bboard, bboard, bboard);
string getWhiteKingMoves(bboard, bboard);

namespace BoardMask {
    bboard rank8 = 18374686479671623680ULL;
    bboard rank7 = 71776119061217280ULL;
    bboard rank6 = 280375465082880ULL;
    bboard rank5 = 1095216660480ULL;
    bboard rank4 = 4278190080ULL;
    bboard rank3 = 16711680ULL;
    bboard rank2 = 65280ULL;
    bboard rank1 = 255ULL;
    
    bboard fileH = 9259542123273814144ULL;
    bboard fileG = 4629771061636907072ULL;
    bboard fileF = 2314885530818453536ULL;
    bboard fileE = 1157442765409226768ULL;
    bboard fileD = 578721382704613384ULL;
    bboard fileC = 289360691352306692ULL;
    bboard fileB = 144680345676153346ULL;
    bboard fileA = 72340172838076673ULL;
}

int main() {
    string stringBoard = 
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "-------X"
        "-------X";
    bboard testBoard = 0;
    getBBoard(testBoard, stringBoard);
    cout << testBoard << endl;
    
    bboard WP = 0, WR = 0, WN = 0, WB = 0, WQ = 0, WK = 0;
    bboard BP = 0, BR = 0, BN = 0, BB = 0, BQ = 0, BK = 0;
    getInitialBoardState(WP, WR, WN, WB, WQ, WK, BP, BR, BN, BB, BQ, BK);
    //printBBoard((WP<<7)&~BoardMask::fileA);
    //printBBoard((WP<<9)&~BoardMask::fileH);
    printBoardState(WP, WR, WN, WB, WQ, WK, BP, BR, BN, BB, BQ, BK);
    getWhiteMoves(WP, WR, WN, WB, WQ, WK, BP, BR, BN, BB, BQ, BK);
}

bboard getWhiteMoves(bboard WP, bboard WR, bboard WN, bboard WB, bboard WQ, bboard WK,
                         bboard BP, bboard BR, bboard BN, bboard BB, bboard BQ, bboard BK) {
    string validMoves;
    bboard WMask = WP|WR|WN|WB|WQ|WK;
    bboard BMask = BP|BR|BN|BB|BQ|BK;
    bboard empty = ~(WMask|BMask);

    // string whitePawnMoves = getWhitePawnMoves(WP, BMask, empty);
    // cout << whitePawnMoves << endl;
    string whiteKingMoves = getWhiteKingMoves(WK, WMask);
    cout << whiteKingMoves << endl;
    return 0;
}


string getWhiteKingMoves(bboard WK, bboard WMask) {
    using namespace BoardMask;
    string validMoves;
    string start;
    for (int i = 0; i < 64; i++) {
        if ((WK>>i)&1) {
            start += 97 + i%8;
            start += to_string(i/8);
            break;
        }
    }

    bboard north = (WK<<8)&~WMask&~rank1;
    bboard northEast = (WK<<7)&~WMask&~rank1&~fileH;
    bboard northWest = (WK<<9)&~WMask&~rank1&~fileA;
    bboard east = (WK>>1)&~WMask&~fileH;
    bboard west = (WK<<1)&~WMask&~fileA;
    bboard south = (WK>>8)&~WMask&~rank8;
    bboard southEast = (WK>>9)&~WMask&~rank8&~fileH;
    bboard southWest = (WK>>7)&~WMask&~rank8&~fileA;

    // TODO: castle

    bboard all = north|northEast|northWest|east|west|south|southEast|southWest;

    for (int i = 0; i < 64; i++) {
        if ((all>>i)&1) {
            validMoves += start;
            validMoves += 97 + i%8;
            validMoves += to_string(i/8);
            validMoves += ',';
        }
    }
    return validMoves;
}

// bboard getWhitePawnMoves(bboard WP, bboard WR, bboard WN, bboard WB, bboard WQ, bboard WK,
//                          bboard BP, bboard BR, bboard BN, bboard BB, bboard BQ, bboard BK) {
string getWhitePawnMoves(bboard WP, bboard BMask, bboard empty) {

    using namespace BoardMask;

    // vector<string> validMoves;
    string validMoves;
    // bboard WMask = WP|WR|WN|WB|WQ|WK;
    // bboard BMask = BP|BR|BN|BB|BQ|BK;
    // bboard empty = ~(WMask|BMask);

    bboard takeLeft = (WP<<7)&~fileH&BMask;  // &~BK?
    printBBoard(takeLeft);
    for (int i = 0; i < 64; i++) {
        if ((takeLeft>>i)&1) {
            validMoves += 97 + (i+1)%8;
            validMoves += to_string(i/8);
            validMoves += 97 + i%8;
            validMoves += to_string(1+i/8);
            validMoves += ",";
        }
    }
    cout << validMoves << endl;

    bboard takeRight = (WP<<9)&~fileA&BMask;
    printBBoard(takeRight);
    for (int i = 0; i < 64; i++) {
        if ((takeRight>>i)&1) {
            validMoves += 97 + (i-1)%8;
            validMoves += to_string(i/8);
            validMoves += 97 + i%8;
            validMoves += to_string(1+i/8);
            validMoves += ",";
        }
    }
    cout << validMoves << endl;
    bboard upOne = (WP<<8)&empty;  // Can't be at top rank
    printBBoard(upOne);
    for (int i = 0; i < 64; i++) {
        if ((upOne>>i)&1) {
            validMoves += 97 + i%8;
            validMoves += to_string(i/8);
            validMoves += 97 + i%8;
            validMoves += to_string(1+i/8);
            validMoves += ",";
        }
    }
    cout << validMoves << endl;
    bboard upTwo = ((WP&rank2)<<16)&empty&(empty<<8);
    printBBoard(upTwo);
    for (int i = 0; i < 64; i++) {
        if ((upTwo>>i)&1) {
            cout << i << endl;
            validMoves += 97 + i%8;
            validMoves += to_string(-1+i/8);
            validMoves += 97 + i%8;
            validMoves += to_string(1+i/8);
            validMoves += ",";
        }
    }
    cout << validMoves << endl;
    return validMoves;
}

void printBoardState(bboard WP, bboard WR, bboard WN, bboard WB, bboard WQ, bboard WK,
                     bboard BP, bboard BR, bboard BN, bboard BB, bboard BQ, bboard BK) {
    string stringBoard = 
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------";
    for (unsigned int i = 0; i < 64; i++) {
        bboard position = 1ULL << (63-i);
        int j = 8*(i/8)+7-(i%8);  // little endian
        if (WP >= position) {
            stringBoard[j] = 'P';
            WP -= position;
        } else if (WR >= position) {
            stringBoard[j] = 'R';
            WR -= position;
        } else if (WN >= position) {
            stringBoard[j] = 'N';
            WN -= position;
        } else if (WB >= position) {
            stringBoard[j] = 'B';
            WB -= position;
        } else if (WQ >= position) {
            stringBoard[j] = 'Q';
            WQ -= position;
        } else if (WK >= position) {
            stringBoard[j] = 'K';
            WK -= position;
        } else if (BP >= position) {
            stringBoard[j] = 'p';
            BP -= position;
        } else if (BR >= position) {
            stringBoard[j] = 'r';
            BR -= position;
        } else if (BN >= position) {
            stringBoard[j] = 'n';
            BN -= position;
        } else if (BB >= position) {
            stringBoard[j] = 'b';
            BB -= position;
        } else if (BQ >= position) {
            stringBoard[j] = 'q';
            BQ -= position;
        } else if (BK >= position) {
            stringBoard[j] = 'k';
            BK -= position;
        } else {
            stringBoard[j] = '-';
        }
    }
    for (unsigned int i = 0; i < stringBoard.length(); i++) {
        const char c = stringBoard[i];
        cout << c << ' ';
        if (i % 8 == 7) {
            cout << endl;
        }
    }
    cout << endl;
    cout << endl;
    return;
}

void getInitialBoardState(bboard& WP, bboard& WR, bboard& WN, bboard& WB, bboard& WQ, bboard& WK,
                          bboard& BP, bboard& BR, bboard& BN, bboard& BB, bboard& BQ, bboard& BK) {
    string stringBoard = 
        "rnbqkbnr"
        "pppppppp"
        "--------"
        "--------"
        "--r-Q---"
        "rP---q-K"
        "PPPPPPPP"
        "RNBQ---R";
    // bboard WP = 0, WR = 0, WN = 0, WB = 0, WQ = 0, WK = 0;
    // bboard BP = 0, BR = 0, BN = 0, BB = 0, BQ = 0, BK = 0;
    bboard position = 1ULL << 63;
    for (unsigned int i = 0; i < stringBoard.length(); i++) {
        const char c = stringBoard[8*(i/8)+7-(i%8)]; // little endian
        //const char c = stringBoard[i];
        switch(c) {
            case 'P': WP += position;
                break;
            case 'p': BP += position;
                break;
            case 'R': WR += position;
                break;
            case 'r': BR += position;
                break;
            case 'N': WN += position;
                break;
            case 'n': BN += position;
                break;
            case 'B': WB += position;
                break;
            case 'b': BB += position;
                break;
            case 'Q': WQ += position;
                break;
            case 'q': BQ += position;
                break;
            case 'K': WK += position;
                break;
            case 'k': BK += position;
                break;
        }
        position = position >> 1;
    }
    return;
}

void getBBoard(bboard& board, string stringBoard) {
    bboard position = 1ULL << 63;
    for (unsigned int i = 0; i < stringBoard.length(); i++) {
        const char c = stringBoard[8*(i/8)+7-(i%8)]; // little endian
        if (c != '-') {
            board += position;
        }
        position = position >> 1;
    }
    printBBoard(board);
    return;
}

// void printBBoard(bboard board) {
//     for (unsigned int i = 0; i < 64; i++) {
//         int j = 8*(i/8)+7-(i%8); // little endian
//         cout << j << endl;
//         bboard position = 1ULL << (63-j);
//         if (board >= position) {
//             cout << "X ";
//             board -= position;
//         } else {
//             cout << "- ";
//         }
//         if (j % 8 == 7) {
//             cout << endl;
//         }
//     }
//     cout << endl;
//     return;
// }


void printBBoard(bboard board) {
    string stringBoard = 
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------"
        "--------";
    for (unsigned int i = 0; i < 64; i++) {
        int j = 8*(i/8)+7-(i%8);  // little endian
        bboard position = 1ULL << (63-i);
        if (board >= position) {
            stringBoard[j] = 'X';
            board -= position;
        } else {
            stringBoard[j] = '-';
        }
    }
    for (unsigned int i = 0; i < stringBoard.length(); i++) {
        const char c = stringBoard[i];
        cout << c << ' ';
        if (i % 8 == 7) {
            cout << endl;
        }
    }
    cout << endl;
    return;
}
