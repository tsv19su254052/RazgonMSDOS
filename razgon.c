# coding: Cp866 -> koi-8r -> Cp1251
/*  *****************************************************
    *        ��������� �������� ������ �������         *
    *---------------------------------------------------*
    *          ���������� ������� ������                *
    *****************************************************/

#include <stdio.h>
#include <math.h>
#include <conio.h>

// todo ������� ������ ������������

// ����� ������� ������������� � ��� �������
class aa{
    // ������ �� ������� ������������� � ���� ���������
    struct Sys{
		long double   // ������
			b = 1.,     // ������������ �����������
			T[3],     // ���������� ������� T0...T2, ���.
			tau = 60.,  // ����� ������������, ���.
			x_min = 0., // �����������, ����.
			x_max = 99.;// ----||------, ����.
    } *psys;

    unsigned int i,   // ����� ����� �� �������
         mi,  // �������
         nt,  // ����� ��������� ����� �������
         is;  // ����� ����� ��� ������� �� ����

    long double k[5],         // ������������ �������-����������� ���������
                a[5],         // ������������ ����������������� ���������
                r,            // ����������� �����������
                tp,           // ����� ��������
                dt;           // ����� �� �������
		
    // ����������� ������
    aa::aa(void){
        // ����� ���������� ����� � �������-���������� ���������
        is = 3;
        // ��������� ����� ��� ���� �� �������
        nt = 579;
        r = 0.;
    }

    // ������������ ����������������� ���������
    public: 
    void coeff_a(void){
        a[4] = psys->T[0] * psys->T[1] * psys->T[2];
        a[3] = psys->T[0] * psys->T[1] + psys->T[0] * psys->T[2] + psys->T[1] * psys->T[2];
        a[2] = psys->T[0] + psys->T[1] + psys->T[2];
        a[1] = 1.;
        a[0] = 0.;
        return;
    }

    // ������������ �������-����������� ���������
    private:
    void coeff_k(void){
        long double c = a[1] + a[2] / dt + a[3] / dt / dt + a[4] / dt / dt / dt;
        k[0] = -a[0] / c;
        k[1] = b / c;
        k[2] = a[2] / dt;
        k[2] += 2. * a[3] / dt / dt;
        k[2] += 3. * a[4] / dt / dt / dt;
        k[2] /= c;
        k[3] = -a[3] / dt / dt;
        k[3] -= 3. * a[4] / dt / dt / dt;
        k[3] /= c;
        k[4] = a[4] / dt / dt / dt / c;
        return;
    }

    // ������� �����
    public:
    long double cicle(void){
        long double S_graph = 0., // ������� ��� ������
                    t_r = 0.;     // �����
        // ��� �� �������
        dt = tp / (long double)(nt * is);

        // ����� ������ ������������
        unsigned int d = (unsigned int) ceil(psys->tau / dt);
        
        // �������� � ���������� ������ � �������� ���
        long double* x = new long double[d+4];
        for(mi = 0; mi <= d + 3; mi++)
            x[mi] = 0.;

        coeff_k();
        do{
            x[d+3] = k[1] * r;
            x[d+3] += k[2] * x[d+2];
            x[d+3] += k[3] * x[d+1];
            x[d+3] += k[4] * x[d];
            // ������� ������� ��� ������
            for(mi = 1; mi <= d + 3; mi++)
                x[mi-1] = x[mi];
            S_graph += fabs(dt * (x[0] + x[1]) / 2.);
            t_r += dt;
        }while(t_r <= tp);
        delete [] x;
        return S_graph;
    }

    // ������ �������
    public:
    void rasgon(void){
        long double S_graph_0, // ������� ��� 1-�� ������
                    S_graph_1, // ������� ��� 2-�� ������
                    ras;       // �������� � ��������
        
        // �������� ����� � 3-�
        is = 3;
        
        // ��� ��������� �� ����� �������
        r = 1. / b;
        
        // ������ ������ �����
        S_graph_0 = cicle();
        do{
            // ��������� ����� ����� �������
            is *= 2;
            // ������ ������ �����
            S_graph_1 = cicle();
            // ������� ������������ �������
            ras = S_graph_1 - S_graph_0;
            // �������� ����� ������ � ������
            S_graph_0 = S_graph_1;
            // ��������
            sound(2000);
            delay(200);
            sound(3000);
            delay(500);
            nosound();
        }while(1);
        return;
    }

    // ������ ������ �� �����
    public:
    void read_dat(void){
        // ������ � ����� "zsr_asu.dat"
        FILE* io = fopen(file_name, "rb");
        if(io == NULL)
            opros();
        io = fopen(file_name, "wb");
            fwrite(psys, sizeof(Sys), 1, io);
        else{
            fread(psys, sizeof(Sys), 1, io);
        }
        fclose(io);
        return;
    }

    // ���������� ������ � ����
    public:
    void record_dat(void){
        FILE* io = fopen(file_name, "wb");
        fwrite(psys, sizeof(Sys), 1, io);
        fclose(io);
    }

    // ���� �������� ������
    public:
    void opros(void){
// todo �������� ���� ���� �������� ������ � ���������� �����������
    }
} *paa;  // ��������� �� ����� �������

void  main(void){
    char pp = 'b';
    char* file_name = "zsr_asu.dat";
    paa->read_dat();
    do{
    	paa->coeff_a();
    	switch(pp){
    		case 'a':
    			paa->opros();
    			break;
    	    case 'c':
    	    	paa->rasgon();
    	}
    }while(pp != 'x');
    paa->record_dat();
}
