#include <iostream>
#include <time.h>

#define FREQ 32

#define MOV 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define JUMP 6
#define HALF 15

using namespace std;

class sysCLK{
    private:
        clock_t tImpIni;
        bool bNivel;
    public:
        sysCLK( void ) { tImpIni = clock(); bNivel = false; }
        bool pulso( void ) {
            clock_t tDecorrido = ( ( clock() - tImpIni ) / ( CLOCKS_PER_SEC / 1000 ) );
            if( ! ( tDecorrido <=  ( 1000 / FREQ  ) )  ) { tImpIni = clock(); bNivel = ! bNivel; }
            return( bNivel );
        }
} oCLK;

class RAM{
    private:
        short memoria[ 1024 ];
    public:
        short read( short iCEL ) { return memoria[ iCEL ]; }
        void readAll( void ) { 
			short i; 
			while( i < sizeof( memoria ) / sizeof( memoria[ 0 ] ) ) { 
				cout << hex << "mem[" << i << "]: \t" << memoria[ i++ ] << ", ";
			}
		}
        void write( short iDate, short iCEL ) { memoria[ iCEL ] = iDate; }
} oRAM;

/** --- Instruções ---
 * 1- 0001 == MOV
 * 2- 0010 == ADD
 * 3- 0011 == SUB
 * 4- 0100 == MUL
 * 5- 0101 == DIV
 * 6- 0110 == JUMP
 * 7- 1111 == HALF
 */

class cpu{
    private:
        short iRG[ 4 ];
        short iRI[ 3 ];
        short iPC;
        bool bCont = true;
    public:
        void executar( void ){
			while( bCont ){
				buscar( );
				alu( );
			}
        }
        void buscar( void ){
			if( iPC == 1024 ) { iPC = 0; bCont = false; oRAM.read( 0xf ); }
            else codificar( oRAM.read( ( iPC++ ) ) );
        }
        int codificar( short iIns ){
            if( ( iIns & 0xf00 )>> 8 == 1 )      { iRI[ 0 ] = 1; iRI[ 1 ] = ( ( iIns & 0xf0 )>> 4 ); iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 2 ) { iRI[ 0 ] = 2; iRI[ 1 ] = ( ( iIns & 0xf0 )>> 4 ); iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 3 ) { iRI[ 0 ] = 3; iRI[ 1 ] = ( ( iIns & 0xf0 )>> 4 ); iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 4 ) { iRI[ 0 ] = 4; iRI[ 1 ] = ( ( iIns & 0xf0 )>> 4 ); iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 5 ) { iRI[ 0 ] = 5; iRI[ 1 ] = ( ( iIns & 0xf0 )>> 4 ); iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 6 ) { iRI[ 0 ] = 6; iRI[ 2 ] = ( iIns & 0xf); }
            else if( ( iIns & 0xf00 )>> 8 == 15) { cout << "ERROR! 404" << endl; bCont = false; return 0; }
            else { buscar( ); }
        }
        void alu( void ){
            if ( iRI[ 0 ] == MOV ) { iRG[ iRI[ 1 ] ] = iRI[ 2 ]; }
            else if ( iRI[ 0 ] == ADD ) { iRG[ iRI[ 1 ] ] += iRI[ 2 ]; }
            else if ( iRI[ 0 ] == SUB ) { iRG[ iRI[ 1 ] ] -= iRI[ 2 ]; }
            else if ( iRI[ 0 ] == MUL ) { iRG[ iRI[ 1 ] ] *= iRI[ 2 ]; }
            else if ( iRI[ 0 ] == DIV ) { iRG[ iRI[ 1 ] ] /= iRI[ 2 ]; }
            else if ( iRI[ 0 ] == JUMP ) { iPC = iRI[ 2 ]; }
        }
        int getIRG( int R ) { return iRG[ R ]; }
}oCPU;

class monitor{
    private:
        bool cont = true;
    public:
        int exec( void ) {
            char cEsc;
            while( cont ){
                iBegin:
                cout << "Access memory[1], Write memory[2], Executer CPU[3], Exit[4] - " << endl; cin >> cEsc;
            
                if( cEsc == '1' ) {
					short iMEM; 
					cout << "Location the read memory Data and Cel or Digit -1 to print all memory: " << endl; 
					cin >> iMEM; 
					if( iMEM == -1 ) oRAM.readAll( );
					else cout << hex << "Data: " << oRAM.read( iMEM ); 
					cout << '\n';
				}
                else if ( cEsc == '2') { 
					short iMEM, iCEL; 
					cout << "Digit Data and Celul: " << endl;
					cin >> hex >> iMEM >> iCEL; 
					oRAM.write( iMEM, iCEL ); 
				}
                else if ( cEsc == '3') {
					oCPU.executar();
					cout << "R0: " << oCPU.getIRG( 0 ) << endl;
					cout << "R1: " << oCPU.getIRG( 1 ) << endl;
					cout << "R2: " << oCPU.getIRG( 2 ) << endl;
					cout << "R3: " << oCPU.getIRG( 3 ) << endl;
                }else if ( cEsc == '4' ) 
					return 0;
                else { 
					cout << "Invalid operation!" << endl; 
					goto iBegin; 
				}
                cin.ignore();
                cout << "Press enter to continue ...";
				cin.get();
				system("clear");
            }
        }
}oMonitor;

int main( void ) {
    oMonitor.exec();
}