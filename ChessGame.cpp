#include <iostream>

using namespace std;

class Piece;

class Space {
    public:
        Piece* occupant;
        bool isOccupied;
        string coordinate;
        Space* N;
        Space* S;
        Space* E;
        Space* W;
        Space* NE;
        Space* SE;
        Space* NW;
        Space* SW;
        bool nOccupied;
        bool sOccupied;
        bool wOccupied;
        bool eOccupied;
        bool nwOccupied;
        bool neOccupied;
        bool swOccupied;
        bool seOccupied;
        Space() {
            nOccupied=false;
            sOccupied=false;
            wOccupied=false;
            eOccupied=false;
            neOccupied=false;
            nwOccupied=false;
            seOccupied=false;
            swOccupied=false;
        }
};

Space spaces[64];

class capturedList {
    public:
        string name;
        bool nextFilled;
        bool previousFilled;
        capturedList* next;
        capturedList* previous;
        capturedList(string n): name(n), nextFilled(false), previousFilled(false) {}
        void printList() {
            cout<<" "<<name;
            if (nextFilled) {
                next->printList();
            }
            else {
                cout<<endl;
            }

        }
        void goToFirst() {
            if (!previousFilled) {
                printList();
            }
            else {
                previous->goToFirst();
            }
        }
};

capturedList capturedWhitePieces("White:");
capturedList capturedBlackPieces("Black:");
capturedList* currentWhiteCaptured=&capturedWhitePieces;
capturedList* currentBlackCaptured=&capturedBlackPieces;



class Piece {
    protected:
        Space space;
    public:
        Piece(string Color, int Point, string Name):color(Color), points(Point), name(Name), isCaptured(false) {}
        bool isCaptured;
        string color;
        int points;
        string name;
        bool canMove(Space position);
        virtual bool move(Space moveSpot)=0;
        void setSpaceOccupied() {
            for (int x=0; x<64; x++) {
                if (spaces[x].coordinate==space.coordinate) {
                    spaces[x].isOccupied=true;
                    spaces[x].occupant=this;
                    //cout<<"d"<<endl;
                    break;
                }
                //cout<<"["<<spaces[x].coordinate<<" | "<<space.coordinate<<"] ";
            }
        }
        Space getSpace() {
            return space;
        }
        void setSpace(Space s) {
            space=s;
            setSpaceOccupied();
        }
        bool diagCheck(Space pos, Space position, int dir) {
            //This is the space we're looking for!
            if (pos.coordinate==position.coordinate) {
                //Space is occupied by one of our own pieces, cannot move there
                if (pos.isOccupied && pos.occupant->color==color) {
                    return false;
                }
                //Space either unoccupied or occupied by an enemy, can be moved to
                return true;
            }
            //Space not our target space, occupied by another piece and thus blocking the way
            else if (pos.isOccupied) {
                return false;
            }
            switch(dir) {
                case 0:
                    if (pos.nwOccupied) {
                        return diagCheck(*pos.NW, position, 0);
                    }
                    else {
                        return false;
                    }
                case 1:
                    if (pos.neOccupied) {
                        return diagCheck(*pos.NE, position, 1);
                    }
                    else {
                        return false;
                    }
                case 2:
                    if (pos.seOccupied) {
                        return diagCheck(*pos.SE, position, 2);
                    }
                    else {
                        return false;
                    }
                case 3:
                    if (pos.swOccupied) {
                        return diagCheck(*pos.SW, position, 3);
                    }
                    else {
                        return false;
                    }
            }
            return false;
        }
        bool crossCheck(Space pos, Space position, int dir) {
            //This is the space we're looking for!
            if (pos.coordinate==position.coordinate) {
                //Space is occupied by one of our own pieces, cannot move there
                if (pos.isOccupied) {
                    if (pos.occupant->color==color) {
                        return false;
                    }
                }
                //Space either unoccupied or occupied by an enemy, can be moved to
                return true;
            }
            //Space not our target space, occupied by another piece and thus blocking the way
            else if (pos.isOccupied) {
                return false;
            }
            switch(dir) {
                case 0:
                    if (pos.nOccupied) {
                        return crossCheck(*pos.N, position, 0);
                    }
                    else {
                        return false;
                    }
                case 1:
                    if (pos.eOccupied) {
                        return crossCheck(*pos.E, position, 1);
                    }
                    else {
                        return false;
                    }
                case 2:
                    if (pos.sOccupied) {
                        return crossCheck(*pos.S, position, 2);
                    }
                    else {
                        return false;
                    }
                case 3:
                    if (pos.wOccupied) {
                        return crossCheck(*pos.W, position, 3);
                    }
                    else {
                        return false;
                    }
            }
            return false;
        }
        ~Piece() {
            cout<<name<<" is Dead"<<endl;
        }
};

class Bishop: public Piece {
    public:
        Bishop(string Color, int Point, string Name): Piece(Color, Point, Name) {}
        bool move(Space moveSpot) {
            if (canMove(moveSpot)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool canMove(Space position) {
            if (isCaptured) {
                return false;
            }
            if (diagCheck(space, position, 0)) {
                return true;
            }
            else if (diagCheck(space, position, 1)) {
                return true;
            }
            else if (diagCheck(space, position, 2)) {
                return true;
            }
            else if (diagCheck(space, position, 3)) {
                return true;
            }
            else {
                return false;
            }
        }
};

class Rook: public Piece {
    public:
        Rook(string Color, int Point, string Name): Piece(Color, Point, Name), hasMoved(true) {}
        bool hasMoved;
        bool move(Space moveSpot) {
            if (canMove(moveSpot)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                hasMoved=false;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool canMove(Space position) {
            if (isCaptured) {
                return false;
            }
            if (crossCheck(space, position, 0)) {
                return true;
            }
            else if (crossCheck(space, position, 1)) {
                return true;
            }
            else if (crossCheck(space, position, 2)) {
                return true;
            }
            else if (crossCheck(space, position, 3)) {
                return true;
            }
            else {
                return false;
            }
        }
};

class Queen: public Piece {
    public:
        Queen(string Color, int Point, string Name): Piece(Color, Point, Name) {}
        bool move(Space moveSpot) {
            if (canMove(moveSpot)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool canMove(Space position) {
            if (isCaptured) {
                return false;
            }
            if (diagCheck(space, position, 0)) {
                cout<<1<<endl;
                return true;
            }
            else if (diagCheck(space, position, 1)) {
                cout<<2<<endl;
                return true;
            }
            else if (diagCheck(space, position, 2)) {
                cout<<3<<endl;
                return true;
            }
            else if (diagCheck(space, position, 3)) {
                cout<<4<<endl;
                return true;
            }
            else if (crossCheck(space, position, 0)) {
                cout<<5<<endl;
                return true;
            }
            else if (crossCheck(space, position, 1)) {
                cout<<6<<endl;
                return true;
            }
            else if (crossCheck(space, position, 2)) {
                cout<<7<<endl;
                return true;
            }
            else if (crossCheck(space, position, 3)) {
                cout<<8<<endl;
                return true;
            }
            else {
                return false;
            }
        }
};

class Pawn:public Piece {
    bool firstMove;
    public:
        Pawn(string Color, int Point, string Name): firstMove(true), Piece(Color, Point, Name) {}
        bool move(Space moveSpot) {
            if (canMove(moveSpot)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                firstMove=false;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool canMove(Space position) {
            if (isCaptured) {
                return false;
            }
            if (color=="white") {
                if (space.neOccupied) {
                    if (space.NE->coordinate==position.coordinate) {
                        if (space.NE->isOccupied) {
                            if (space.NE->occupant->color!=color) {
                                return true;
                            }
                        }
                    }
                }
                if (space.nwOccupied) {
                    if (space.NW->coordinate==position.coordinate) {
                        if (space.NW->isOccupied) {
                            if (space.NW->occupant->color!=color) {
                                return true;
                            }
                        }
                    }
                }
                if (space.nOccupied) {
                    if (space.N->coordinate==position.coordinate) {
                        if (space.N->isOccupied==false) {
                            return true;
                        }
                    }
                }
                if (firstMove) {
                    if (space.N->isOccupied==false && space.N->N->coordinate==position.coordinate) {
                        if (space.N->N->isOccupied==false) {
                            return true;
                        }
                    }
                }
            }
            else {
                if (space.seOccupied) {
                    if (space.SE->coordinate==position.coordinate) {
                        if (space.SE->isOccupied) {
                            if (space.SE->occupant->color!=color) {
                                return true;
                            }
                        }
                    }
                }
                if (space.swOccupied) {
                    if (space.SW->coordinate==position.coordinate) {
                        if (space.SW->isOccupied) {
                            if (space.SW->occupant->color!=color) {
                                return true;
                            }
                        }
                    }
                }
                if (space.sOccupied) {
                    if (space.S->coordinate==position.coordinate) {
                        if (space.S->isOccupied==false) {
                            return true;
                        }
                    }
                }
                if (firstMove) {
                    if (space.S->isOccupied==false && space.S->S->coordinate==position.coordinate) {
                        if (space.S->S->isOccupied==false) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
};

class Knight:public Piece {
    public:
        Knight(string Color, int Point, string Name): Piece(Color, Point, Name) {}
        bool move(Space moveSpot) {
            if (canMove(moveSpot)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool canMove(Space position) {
            if (isCaptured) {
                return false;
            }
            if (space.nOccupied) {
                if (space.N->nOccupied) {
                    if (space.N->N->wOccupied && space.N->N->W->coordinate==position.coordinate) {
                        if (space.N->N->W->isOccupied) {
                            if (space.N->N->W->occupant->color!=color) {
                                cout<<1<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<2<<endl;
                            return true;
                        }
                    }
                    if (space.N->N->eOccupied && space.N->N->E->coordinate==position.coordinate) {
                        if (space.N->N->E->isOccupied) {
                            if (space.N->N->E->occupant->color!=color) {
                                cout<<3<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<4<<endl;
                            return true;
                        }
                    }
                }
            }
            if (space.sOccupied) {
                if (space.S->sOccupied) {
                    if (space.S->S->wOccupied && space.S->S->W->coordinate==position.coordinate) {
                        if (space.S->S->W->isOccupied) {
                            if (space.S->S->W->occupant->color!=color) {
                                cout<<5<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<space.S->coordinate<<endl;
                            cout<<space.S->S->coordinate<<endl;
                            cout<<space.S->S->W->coordinate<<endl;
                            cout<<6<<endl;
                            return true;
                        }
                    }
                    if (space.S->S->eOccupied && space.S->S->E->coordinate==position.coordinate) {
                        if (space.S->S->E->isOccupied) {
                            if (space.S->S->E->occupant->color!=color) {
                                cout<<7<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<8<<endl;
                            return true;
                        }
                    }
                }
            }
            if (space.wOccupied) {
                if (space.W->wOccupied) {
                    if (space.W->W->nOccupied && space.W->W->N->coordinate==position.coordinate) {
                        if (space.W->W->N->isOccupied) {
                            if (space.W->W->N->occupant->color!=color) {
                                cout<<9<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<10<<endl;
                            return true;
                        }
                    }
                    if (space.W->W->sOccupied && space.W->W->S->coordinate==position.coordinate) {
                        if (space.W->W->S->isOccupied) {
                            if (space.W->W->S->occupant->color!=color) {
                                cout<<11<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<12<<endl;
                            cout<<position.coordinate<<endl;
                            cout<<space.W->W->S->coordinate<<endl;
                            return true;
                        }
                    }
                }
            }
            if (space.eOccupied) {
                if (space.E->eOccupied) {
                    if (space.E->E->nOccupied && space.E->E->N->coordinate==position.coordinate) {
                        if (space.E->E->N->isOccupied) {
                            if (space.E->E->N->occupant->color!=color) {
                                cout<<13<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<14<<endl;
                            return true;
                        }
                    }
                    if (space.E->E->sOccupied && space.E->E->S->coordinate==position.coordinate) {
                        if (space.E->E->S->isOccupied) {
                            if (space.E->E->S->occupant->color!=color) {
                                cout<<15<<endl;
                                return true;
                            }
                        }
                        else {
                            cout<<16<<endl;
                            return true;
                        }
                    }
                }
            }
            return false;
        }
};

Pawn WPawn1("white", 1, "pawn");
Pawn WPawn2(WPawn1);
Pawn WPawn3(WPawn1);
Pawn WPawn4(WPawn1);
Pawn WPawn5(WPawn1);
Pawn WPawn6(WPawn1);
Pawn WPawn7(WPawn1);
Pawn WPawn8(WPawn1);
Bishop WBishop1("white", 3, "bishop");
Bishop WBishop2("white", 3, "bishop");
Knight WKnight1("white", 3, "knight");
Knight WKnight2("white", 3, "knight");
Rook WRook1("white", 5, "rook");
Rook WRook2("white", 5, "rook");
Queen WQueen("white", 9, "queen");

Pawn BPawn1("black", 1, "pawn");
Pawn BPawn2(BPawn1);
Pawn BPawn3(BPawn1);
Pawn BPawn4(BPawn1);
Pawn BPawn5(BPawn1);
Pawn BPawn6(BPawn1);
Pawn BPawn7(BPawn1);
Pawn BPawn8(BPawn1);
Bishop BBishop1("black", 3, "bishop");
Bishop BBishop2("black", 3, "bishop");
Knight BKnight1("black", 3, "knight");
Knight BKnight2("black", 3, "knight");
Rook BRook1("black", 5, "rook");
Rook BRook2("black", 5, "rook");
Queen BQueen("black", 9, "queen");

class King:public Piece {
    public:
        bool canCastle;
        bool inCheck;
        King(string Color, int Point, string Name): Piece(Color, Point, Name), canCastle(true), inCheck(false) {}
        bool move(Space moveSpot) {return true;}
        bool move(Space moveSpot, King &enemyKing) {
            if (canMove(moveSpot, enemyKing) && notInCheck(moveSpot, enemyKing)) {
                for (int x=0; x<64; x++) {
                    if (spaces[x].coordinate==space.coordinate) {
                        spaces[x].isOccupied=false;
                        break;
                    }
                }
                if (moveSpot.isOccupied) {
                    moveSpot.occupant->isCaptured=true;
                    capturedList item(moveSpot.occupant->name);
                    if (color=="black") {
                        item.previous=currentWhiteCaptured;
                        item.previousFilled=true;
                        currentWhiteCaptured->next=&item;
                        currentWhiteCaptured->nextFilled=true;
                        currentWhiteCaptured=&item;
                        currentWhiteCaptured->goToFirst();
                    }
                    else {
                        item.previous=currentBlackCaptured;
                        item.previousFilled=true;
                        currentBlackCaptured->next=&item;
                        currentBlackCaptured->nextFilled=true;
                        currentBlackCaptured=&item;
                        currentBlackCaptured->goToFirst();
                    }
                }
                for (int y=0; y<64; y++) {
                    if (spaces[y].coordinate==moveSpot.coordinate) {
                        spaces[y].occupant=this;
                        spaces[y].isOccupied=true;
                        break;
                    }
                }
                space=moveSpot;
                canCastle=false;
                cout<<space.coordinate<<endl;
                return true;
            }
            else {
                cout<<"Can't move!"<<endl;
                return false;
            }
        }
        bool notInCheck(Space spot, King &enemyKing) {
            if (color=="black") {
                if (WPawn1.canMove(spot)) {
                    return false;
                }
                else if (WPawn2.canMove(spot)) {
                    return false;
                }
                else if (WPawn3.canMove(spot)) {
                    return false;
                }
                else if (WPawn4.canMove(spot)) {
                    return false;
                }
                else if (WPawn5.canMove(spot)) {
                    return false;
                }
                else if (WPawn6.canMove(spot)) {
                    return false;
                }
                else if (WPawn7.canMove(spot)) {
                    return false;
                }
                else if (WPawn8.canMove(spot)) {
                    return false;
                }
                else if (WBishop1.canMove(spot)) {
                    return false;
                }
                else if (WBishop2.canMove(spot)) {
                    return false;
                }
                else if (WKnight1.canMove(spot)) {
                    return false;
                }
                else if (WKnight2.canMove(spot)) {
                    return false;
                }
                else if (WRook1.canMove(spot)) {
                    return false;
                }
                else if (WRook2.canMove(spot)) {
                    return false;
                }
                else if (WQueen.canMove(spot)) {
                    return false;
                }
                else if (enemyKing.canMove(spot, *this)) {
                    return false;
                }
            }
            else {
                if (BPawn1.canMove(spot)) {
                    return false;
                }
                else if (BPawn2.canMove(spot)) {
                    return false;
                }
                else if (BPawn3.canMove(spot)) {
                    return false;
                }
                else if (BPawn4.canMove(spot)) {
                    return false;
                }
                else if (BPawn5.canMove(spot)) {
                    return false;
                }
                else if (BPawn6.canMove(spot)) {
                    return false;
                }
                else if (BPawn7.canMove(spot)) {
                    return false;
                }
                else if (BPawn8.canMove(spot)) {
                    return false;
                }
                else if (BBishop1.canMove(spot)) {
                    return false;
                }
                else if (BBishop2.canMove(spot)) {
                    return false;
                }
                else if (BKnight1.canMove(spot)) {
                    return false;
                }
                else if (BKnight2.canMove(spot)) {
                    return false;
                }
                else if (BRook1.canMove(spot)) {
                    return false;
                }
                else if (BRook2.canMove(spot)) {
                    return false;
                }
                else if (BQueen.canMove(spot)) {
                    return false;
                }
                else if (enemyKing.canMove(spot, *this)) {
                    return false;
                }
            }
            return true;
        } 
        bool canMove(Space position, King &enemyKing) {
            if (isCaptured) {
                return false;
            }
            if (canCastle) {
                if (color=="black") {
                    if (space.E->E->coordinate==position.coordinate && BRook2.hasMoved==false) {
                        if (BRook2.canMove(*space.E) && notInCheck(*space.E, enemyKing) && notInCheck(*space.E->E, enemyKing)) {
                            BRook2.move(*space.E);
                            return true;
                        }
                    }
                    if (space.W->W->coordinate==position.coordinate && BRook1.hasMoved==false) {
                        if (BRook1.canMove(*space.W) && notInCheck(*space.W, enemyKing) && notInCheck(*space.W->W, enemyKing)) {
                            BRook1.move(*space.W);
                            return true;
                        }
                    }
                }
                else {
                    if (space.E->E->coordinate==position.coordinate && WRook2.hasMoved==false) {
                        if (WRook2.canMove(*space.E) && notInCheck(*space.E, enemyKing) && notInCheck(*space.E->E, enemyKing)) {
                            WRook2.move(*space.E);
                            return true;
                        }
                    }
                    if (space.W->W->coordinate==position.coordinate && WRook1.hasMoved==false) {
                        if (WRook1.canMove(*space.W) && notInCheck(*space.W, enemyKing) && notInCheck(*space.W->W, enemyKing)) {
                            WRook1.move(*space.W);
                            return true;
                        }
                    }
                }
            }
            if (space.nOccupied) {
                if (space.N->coordinate==position.coordinate) {
                    if (space.N->isOccupied) {
                        if (space.N->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.sOccupied) {
                if (space.S->coordinate==position.coordinate) {
                    if (space.S->isOccupied) {
                        if (space.S->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.eOccupied) {
                if (space.E->coordinate==position.coordinate) {
                    if (space.E->isOccupied) {
                        if (space.E->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.wOccupied) {
                if (space.W->coordinate==position.coordinate) {
                    if (space.W->isOccupied) {
                        if (space.W->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.neOccupied) {
                if (space.NE->coordinate==position.coordinate) {
                    if (space.NE->isOccupied) {
                        if (space.NE->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.nwOccupied) {
                if (space.NW->coordinate==position.coordinate) {
                    if (space.NW->isOccupied) {
                        if (space.NW->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.seOccupied) {
                if (space.SE->coordinate==position.coordinate) {
                    if (space.SE->isOccupied) {
                        if (space.SE->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            if (space.swOccupied) {
                if (space.SW->coordinate==position.coordinate) {
                    if (space.SW->isOccupied) {
                        if (space.SW->occupant->color!=color) {
                            return true;
                        }
                    }
                    return true;
                }
            }
            return false;
        }
};

King WKing("white", 2, "King");
King BKing("black", 2, "King");

Space coorToSpace(string coor) {
    for (int x=0; x<64; x++) {
        if (spaces[x].coordinate==coor) {
            return spaces[x];
        }
    }
    cout<<"Error"<<endl;
    return spaces[0];
}

bool playerMovePiece(string="D2", string="D4", string="white");

bool playerMovePiece(string coorStart, string coorEnd, string color) {
    bool moved;
    for (int x=0; x<64; x++) {
        if (spaces[x].coordinate==coorStart) {
            if (spaces[x].isOccupied) {
                if (color=="black") {
                    if (BPawn1.getSpace().coordinate==coorStart) {
                        moved=BPawn1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn2.getSpace().coordinate==coorStart) {
                        moved=BPawn2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn3.getSpace().coordinate==coorStart) {
                        moved=BPawn3.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn4.getSpace().coordinate==coorStart) {
                        moved=BPawn4.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn5.getSpace().coordinate==coorStart) {
                        moved=BPawn5.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn6.getSpace().coordinate==coorStart) {
                        moved=BPawn6.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn7.getSpace().coordinate==coorStart) {
                        moved=BPawn7.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BPawn8.getSpace().coordinate==coorStart) {
                        moved=BPawn8.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BBishop1.getSpace().coordinate==coorStart) {
                        moved=BBishop1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BBishop2.getSpace().coordinate==coorStart) {
                        moved=BBishop2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BKnight1.getSpace().coordinate==coorStart) {
                        moved=BKnight1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BKnight2.getSpace().coordinate==coorStart) {
                        moved=BKnight2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BRook1.getSpace().coordinate==coorStart) {
                        moved=BRook1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BRook2.getSpace().coordinate==coorStart) {
                        moved=BRook2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BQueen.getSpace().coordinate==coorStart) {
                        moved=BQueen.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (BKing.getSpace().coordinate==coorStart) {
                        moved=BKing.move(coorToSpace(coorEnd), WKing);
                        break;
                    }
                }
                else {
                    if (WPawn1.getSpace().coordinate==coorStart) {
                        moved=WPawn1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn2.getSpace().coordinate==coorStart) {
                        moved=WPawn2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn3.getSpace().coordinate==coorStart) {
                        moved=WPawn3.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn4.getSpace().coordinate==coorStart) {
                        moved=WPawn4.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn5.getSpace().coordinate==coorStart) {
                        moved=WPawn5.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn6.getSpace().coordinate==coorStart) {
                        moved=WPawn6.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn7.getSpace().coordinate==coorStart) {
                        moved=WPawn7.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WPawn8.getSpace().coordinate==coorStart) {
                        moved=WPawn8.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WBishop1.getSpace().coordinate==coorStart) {
                        moved=WBishop1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WBishop2.getSpace().coordinate==coorStart) {
                        moved=WBishop2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WKnight1.getSpace().coordinate==coorStart) {
                        moved=WKnight1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WKnight2.getSpace().coordinate==coorStart) {
                        moved=WKnight2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WRook1.getSpace().coordinate==coorStart) {
                        moved=WRook1.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WRook2.getSpace().coordinate==coorStart) {
                        moved=WRook2.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WQueen.getSpace().coordinate==coorStart) {
                        moved=WQueen.move(coorToSpace(coorEnd));
                        break;
                    }
                    else if (WKing.getSpace().coordinate==coorStart) {
                        moved=WKing.move(coorToSpace(coorEnd), BKing);
                        break;
                    }
                }
            }
        }
    }
    return moved;
}

void printBoard() {
    int num=8;
    cout<<num<<" ";
    for (int x=0; x<64; x++) {
        if (spaces[x].isOccupied) {
            cout<<spaces[x].occupant->color[0]<<spaces[x].occupant->name[0]<<" ";
        }
        else {
            cout<<spaces[x].coordinate<<" ";
        }
        if ((x+1)%8==0 && x!=0) {
            num--;
            cout<<endl;
            if (num>0) {
                cout<<num<<" ";
            }
            else {
                cout<<"  ";
            }
        }
    }
    string alpha[8]={"A","B","C","D","E","F","G","H"};
    for (int y=0; y<8; y++) {
        cout<<alpha[y]<<"  ";
    }
    cout<<endl;
}

class Player {
    public:
        int points;
        static int numTurns;
        const string color;
        char* name;
        Player(string n): points(0), color("white") {
            name=new char[n.length()];
            strcpy(name, n.c_str());
            cout<<"white player name: ";
            for (int i=0; i<n.length(); i++) { 
                cout << name[i]; 
            }
            cout<<endl;
        }
        Player(string c, string n): points(0), color(c) {
            name=new char[n.length()];
            strcpy(name, n.c_str());
            cout<<color<<" player name: ";
            for (int i=0; i<n.length(); i++) { 
                cout << name[i]; 
            }
            cout<<endl;
        }
        bool inCheck() {
            if (color=="black") {
                if (!BKing.notInCheck(BKing.getSpace(), WKing)) {
                    return true;
                }
            }
            else {
                if (!WKing.notInCheck(WKing.getSpace(), BKing)) {
                    return true;
                }
            }
            return false;
        }
        void increaseNumTurns() {
            numTurns++;
        }
        void operator ++ () {
            increaseNumTurns();
        }
        int takeTurn() {

            cout<<color<<" player's turn!"<<endl;

            string start;
            string end;

            cout<<"Enter space to move from or type 'resign' to quit:"<<endl;
            cin>>start;

            if (start=="resign" || start=="Resign") {
                cout<<(color=="black"?"Black":"White")<<" player resigns; "<<(color=="black"?"White":"Black")<<" player is victorious!"<<endl;
                cout<<"Total number of turns taken: "<<numTurns<<endl;
                
                return 0;
            }

            cout<<"Enter space to move to or type 'resign' to quit:"<<endl;
            cin>>end;

            if (end=="resign" || end=="Resign") {
                cout<<(color=="black"?"Black":"White")<<" player resigns; "<<(color=="black"?"White":"Black")<<" player is victorious!"<<endl;
                cout<<"Total number of turns taken: "<<numTurns<<endl;
                return 0;
            }

            if (!playerMovePiece(start, end, color)) {
                takeTurn();
                return 0;
            }
            printBoard();
            if (inCheck()) {
                cout<<"Checkmate!"<<(color=="black"?"White":"Black")<<" player is victorious!"<<endl;
                cout<<"Total number of turns taken: "<<numTurns<<endl;
                return 0;
            }
            nextTurn();
            return 0;
        }
        
        void nextTurn();

        ~Player() {
            delete[] name;
        }
};

int Player::numTurns=0;

Player WhitePlayer("Sarah");
Player BlackPlayer("black", "Bob");

void Player::nextTurn() {
    if (color=="black") {
        ++WhitePlayer;
        WhitePlayer.takeTurn();
    }
    else {
        ++BlackPlayer;
        BlackPlayer.takeTurn();
    }
}


int main() {

    string alpha[8]={"A","B","C","D","E","F","G","H"};
    int pointIndex=0;
    for (int y=0; y<8; y++) {
        for (int x=0; x<8; x++) {
            Space pnt;
            pnt.coordinate=alpha[x]+to_string(8-y);
            spaces[pointIndex]=pnt;
            pointIndex++;
        }
    }

    for (int z=0; z<64; z++) {
        //NW, W, SW
        if (z%8!=0) {
            spaces[z].W=&spaces[z-1];
            spaces[z].wOccupied=true;
            if (z>7) {
                spaces[z].NW=&spaces[z-9];
                spaces[z].nwOccupied=true;
            }
            if (z<56) {
                spaces[z].SW=&spaces[z+7];
                spaces[z].swOccupied=true;
            }
        }
        //NE, E, SE
        if (z%8!=7) {
            spaces[z].E=&spaces[z+1];
            spaces[z].eOccupied=true;
            if (z<56) {
                spaces[z].SE=&spaces[z+9];
                spaces[z].seOccupied=true;
            }
            if (z>7) {
                spaces[z].NE=&spaces[z-7];
                spaces[z].neOccupied=true;
            }
        }
        //N
        if (z>7) {
            spaces[z].N=&spaces[z-8];
            spaces[z].nOccupied=true;
        }
        //S
        if (z<56) {
            spaces[z].S=&spaces[z+8];
            spaces[z].sOccupied=true;
        }
    }

    WPawn1.setSpace(spaces[48]);
    WPawn2.setSpace(spaces[49]);
    WPawn3.setSpace(spaces[50]);
    WPawn4.setSpace(spaces[51]);
    WPawn5.setSpace(spaces[52]);
    WPawn6.setSpace(spaces[53]);
    WPawn7.setSpace(spaces[54]);
    WPawn8.setSpace(spaces[55]);
    WBishop1.setSpace(spaces[58]);
    WBishop2.setSpace(spaces[61]);
    WKnight1.setSpace(spaces[57]);
    WKnight2.setSpace(spaces[62]);
    WRook1.setSpace(spaces[56]);
    WRook2.setSpace(spaces[63]);
    WQueen.setSpace(spaces[59]);
    WKing.setSpace(spaces[60]);

    BPawn1.setSpace(spaces[8]);
    BPawn2.setSpace(spaces[9]);
    BPawn3.setSpace(spaces[10]);
    BPawn4.setSpace(spaces[11]);
    BPawn5.setSpace(spaces[12]);
    BPawn6.setSpace(spaces[13]);
    BPawn7.setSpace(spaces[14]);
    BPawn8.setSpace(spaces[15]);
    BBishop1.setSpace(spaces[2]);
    BBishop2.setSpace(spaces[5]);
    BKnight1.setSpace(spaces[1]);
    BKnight2.setSpace(spaces[6]);
    BRook1.setSpace(spaces[0]);
    BRook2.setSpace(spaces[7]);
    BQueen.setSpace(spaces[3]);
    BKing.setSpace(spaces[4]);

    printBoard();

    WhitePlayer.takeTurn();

    return 0;
}