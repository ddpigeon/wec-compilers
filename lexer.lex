%{

%}

%%
[ \t\n]                                 ; /*ignore whitespaces*/
[A-Z][a-zA-Z]*                          {return 1;}
[A-Za-z0-9]*[0-9][A-Za-z0-9]*           {return 2;}
[a-z][a-zA-Z]*                          {return 3;}
[.!?]                                   {return 4;}
.                                       {return 0;}

%%

int yywrap(){
    return 1;
}
