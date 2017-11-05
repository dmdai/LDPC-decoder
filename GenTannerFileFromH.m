%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   File Name: GenTannerFileFromH.m 
%   Purpose: Genate Tannaer file from a given parity-check matrix H of small size 
%            and demonstrate the file format for students. 
%   Inputs:  Parity-check matrix H
%   Outputs:  Tanner File.txt, the name of which appears at line 23. 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear;
H = [1 1 1 0 0 0;
     0 1 0 1 0 1;
     0 0 1 1 1 0;
     1 0 0 0 1 1];
[m,n]=size(H)

dv_deg=sum(H,1) 
dc_deg=sum(H,2) 
dvmax=max(dv_deg)
dcmax=max(dc_deg)

fid = fopen('H6_4Reg23.txt','w');
fprintf(fid,'%d %d\n',n,m);
fprintf(fid,'%d %d\n',dvmax,dcmax);

for col=1:n
    fprintf(fid,'%d ',dv_deg(col));
end
fprintf(fid,'\n');

for row=1:m
    fprintf(fid,'%d ',dc_deg(row));
end
fprintf(fid,'\n');

[I,J]=find(H);

k=1;
for col=1:n
    for row=1:dv_deg(col)
        if (row==dv_deg(col))
            fprintf(fid,'%d',I(k));
            fprintf(fid,'\n');
        else
            fprintf(fid,'%d ',I(k));
        end
        k=k+1;
    end
end
[I,J]=find(H');
k=1;
for row=1:m
    for col=1:dc_deg(row)
        if (col==dc_deg(row))
            fprintf(fid,'%d',I(k));
            fprintf(fid,'\n');
        else
            fprintf(fid,'%d ',I(k));
        end
        k=k+1;
    end
end
fclose(fid);
