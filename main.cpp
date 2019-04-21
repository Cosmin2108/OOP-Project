#include <iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include<conio.h>

std::ifstream f("Date.txt");

enum class tip
{
    Complex=0,
    Punct,
    ObArray,
    Object,
};

class Object
{
private:
    virtual void display(){}
    virtual void citire(){}

public:
    tip t;
    Object():t(tip::Object){}
    Object(tip t):t(t){}
    Object(Object &ob): t(ob.t){}
    virtual ~Object(){}
    friend std::istream& operator >>(std::istream& in, Object &ob);
    friend std::ostream& operator <<(std::ostream& out, Object &ob);
};


class Punct:public Object
{
private:
    double x,y;
    void display();
    void citire();


public:
    Punct(double a,double b):Object(tip::Punct){x=a;y=b;}
    Punct():Object(tip::Punct){x=0.0;y=0.0;}
    Punct(Punct &ob):Object(ob){x=ob.x;y=ob.y;}
    ~Punct(){}

    friend std::istream& operator >>(std::istream& in, Punct &pc);
    friend std::ostream& operator <<(std::ostream& out, Punct &pc);
    Punct& operator =(Punct &ob);
};

class Complex:public Object
{
private:
    void display();
    void citire ();
    double re, im;
public:
    Complex():Object(tip::Complex)
    {
        re=0.0;
        im=0.0;
    }
    Complex(double x,double y);
    Complex(Complex &ob);

    ~Complex(){ re=0.0; im=0.0;}
    friend std::istream& operator >>(std::istream& in, Complex &c) { c.display();}
    friend std::ostream& operator <<(std::ostream& out, Complex &c);
    Complex& operator=(Complex &ob);
};

 Complex::Complex(Complex &ob):Object(ob)         /// cpy constr
{
        re=ob.re;
        im=ob.im;
}

 Complex:: Complex(double x,double y):Object(tip::Complex)
{  std::cout<<"Constructor complex cu param\n";
    re=x;
    im=y;
}

class ObArray:public Object
{
private:
   Object **o;                   /// am vector de pointeri deoarece obiectele sunt de dimensiuni/tipuri diferite si nu pot aloca ceva fix
   int dimens;                  /// dimensiune vector
   int sizee;                   /// dim curenta
   int grows;                   /// increment de crestere gen ++, cred
   void display();
   void citire();
public:
   ObArray():Object(tip::ObArray){}
   ObArray(int dim,int sz, int grw):Object(tip::ObArray)
   {   dimens=dim;
       sizee=sz;
       grows=grw;
       o=new Object*[dimens];

   }
   ~ObArray()
   {
        if(o)
      {
         for(int i=0;i<sizee;i++)
	        if(o[i]!=NULL)
	          delete o[i];

         delete []o;
       }
   }

   ObArray(ObArray &ob); /// constructor de copiere
   int addOb(Object *Ob, char *s);
   void RemoveAll();
   int getSize();
   int insertAt(int i, Object *x);
   int removeAt(int i);
   Object* getAt(int i);
   void resizee();
   friend std::istream& operator >>(std::istream& in, ObArray &Ob) { Ob.display(); return in;}
   friend std::ostream& operator <<(std::ostream& out, ObArray &Ob);
   ObArray& operator =(ObArray &ob);

};

void ObArray::display()
{
    if(o)
     for(int i=0;i<sizee;i++)
        if (o[i]!=NULL)
          std::cout<<*(o[i])<<"\n";
}

Punct& Punct::operator =(Punct &ob)
{
    x=ob.x;
    y=ob.y;
    return *this;
}

Complex& Complex::operator=(Complex &ob)
{
    re=ob.re;
    im=ob.im;

   return *this;
}

void ObArray::RemoveAll()
{
    if(o)
    {
       for(int i=0;i<sizee;i++)
        if(o[i]!=NULL)
			delete o[i];
     delete []o;
    }
	dimens=0;  sizee=0;  grows=0;

}

ObArray::ObArray(ObArray &ob):Object(ob)  /// cpy
{
  dimens=ob.dimens+ob.grows;   sizee=ob.sizee;   grows=ob.grows;
  o=new Object*[dimens];

  for(int i=0;i<sizee;i++)
       {
         if((*(ob.o[i])).t==tip::Complex)
           {
               o[i]=new Complex;
               *(Complex*)(o[i])=*(Complex*)(ob.o[i]);
           }
         else
         if((*(ob.o[i])).t==tip::Punct)
         {
              o[i]=new Punct;
              *(Punct*)(o[i])=*(Punct*)(ob.o[i]);
         }

       }

}

ObArray& ObArray::operator =(ObArray &ob)
{
  for(int i=0;i<sizee;i++)
      if(o[i])
        delete o[i];
   delete []o;

  dimens=ob.dimens;
  sizee=ob.sizee;
  grows=ob.grows;
  o=new Object*[dimens];

  for(int i=0;i<sizee;i++)
     if(ob.o[i]!=NULL)
     {
         if((*(ob.o[i])).t==tip::Complex)
           {
               o[i]=new Complex;
               *(Complex*)(o[i])=*(Complex*)(ob.o[i]);
           }
         else
         if((*(ob.o[i])).t==tip::Punct)
         {
              o[i]=new Punct;
              *(Punct*)(o[i])=*(Punct*)(ob.o[i]);
         }
     }

  return *this;

}

void Punct::display()
{
    std::cout<<"Punct: ( "<<x<<" , "<<y<<" )\n";
}

void Complex::display()
{
    std::cout<<"Numar complex: "<<re<<" + "<<im<<"i\n";
}

std::ostream& operator <<(std::ostream& out, Object &ob)
{
    ob.display();
    return out;
}

void Punct::citire()
{ std:: cout<<"Coordonate x si y: ";
 std::cin>>x>>y;
}

 void Complex::citire ()
 { std::cout<<"Partea reala si imaginara a nr. complex: ";
   std::cin>>re>>im;
 }

 std::istream& operator >>(std::istream& in, Object &ob)
{
    ob.citire();
    return in;
}

void ObArray::resizee()
 {
     if(dimens==0)
        grows=5;
     else
        grows=dimens;
	 ObArray aux=(*this); /// am nevoie de cpy constr pt ObArray
     (*this)=aux;

     if((*this).dimens==grows)
        throw "Bad_realloc!";
     grows=0;

 }

int ObArray::addOb(Object *Ob, char *s)
{
  try{
      if(sizee>=dimens)
        (*this).resizee();

      if((*Ob).t==tip::Complex)
       {
         o[sizee]= new Complex;

         *(Complex*)(o[sizee])=*(Complex*)Ob;
       }
      else
      if((*Ob).t==tip::Punct)
      {
       o[sizee]= new Punct;
       *(Punct*)(o[sizee])=*(Punct*)Ob;
      }

     sizee+=1;
     return 1;
    }
      catch(const char*s)
       { std::cout<<s<<"\n"; }
     return 0;
}


void ObArray::citire()
{   int ok=1;
    char s[12];
   while(1)
   { std::cout<<"Ce fel de obiect vrei sa adaugi?\nNumeste tip(Complex/Punct) sau apasa 'x' pentru a finaliza: ";
     std::cin>>s;
     std::cout<<"\n";

     if(strcmp(s,"Complex")==0)
        {
         Object *c=new Complex;
         std::cin>>*c;
         ok=(*this).addOb(c,s);
         system("cls");
        }
      else
        if(strcmp(s,"Punct")==0)
      {
          Object *p=new Punct;
         std::cin>>*p;
         ok=(*this).addOb(p,s);
         system("cls");
      }
      else
        if(strcmp(s,"x")==0)
        {
          system("cls");
          return;
        }
       else
        {
            std::cout<<"Obiectul nu a fost inca definit\n";
            system("cls");
        }
   }

}

int ObArray::getSize()
{
    return sizee;
}

Object* ObArray::getAt(int i)
{
  if(i<dimens)
  return o[i];
  else
  return NULL;
}

int ObArray::insertAt(int i, Object *x)
{
   if(i<=dimens)
   {
      if(sizee<dimens-1)
          (*this).resizee();

      for(int j=dimens-1;j>i;j--)
      {  o[j]=o[j-1];
         o[j-1]=NULL;
      }
       if((*x).t==tip::Complex)
       {
           o[i]=new Complex;
           *(Complex*)(o[i])=*(Complex*)x;
       }
       else
       if((*x).t==tip::Punct)
       {
           o[i]=new Punct;
           *(Punct*)(o[i])=*(Punct*)x;
       }

       sizee+=1;
   }
    else
     return 0;
  return 1;
}

 int ObArray::removeAt(int i)
 {
   int j;
   if(i<sizee)
   {
     delete o[i];
     for( j=i;j<sizee-1;j++)
	   { o[j]=NULL; o[j]=o[j+1]; }

     o[j]=NULL;
	 sizee-=1;
   }
   else
	return 0;
	 return 1;
 }

 void meniu()
 {
     Sleep(1500);
     system("cls");
     std::cout<<"||||||||||||||||||||||||||||||||||||||||||\n";
     std::cout<<"|| Optiuni:                             ||\n";
	 std::cout<<"|| 1-verifica dimensiunea listei        ||\n|| 2-extrage element de pe o pozitie    ||\n|| 3-elimina element de pe o pozitie    ||\n|| 4-adauga element pe o pozitie        ||\n|| 5-sterge toata lista                 ||\n|| 6-incheie ultimele modificari        ||\n";
     std::cout<<"||||||||||||||||||||||||||||||||||||||||||\n";

 }

int main()
{
    Object *arr=new ObArray(1,0,0); /// sau ObArray x(nr,poz,contor); Object *arr=&x; !PASTREZ
    std::cin>>*arr;
    std::cout<<"Lista ta este: \n";
    std::cout<<*arr;

    char *s=new char[3], *tip_el=new char[15];
	std::cout<<"Vrei sa mai faci modificar?\nDa/Nu: ";
    std::cin>>s;
	if(strcmp(s,"Da")==0)
    {
      meniu();
      Object *ob;
      ObArray *aux=static_cast<ObArray*>(arr);
	  while(1)
	  {  ///std::cin>>s;
         switch(_getch())
		 {
			 case '1':{
              int dim=(*aux).getSize(); std::cout<<"Dimensiunea listei: "<<dim<<"\n";
              meniu();
              break;
              }

			 case '2':{
              int poz;
			  std::cout<<"Introdu pozitia: ";
			  std::cin>>poz;
			  ob=(*aux).getAt(poz);
			  if(ob!=NULL)
                std::cout<<"Elementul este: "<<*ob<<"\n";
              else
                std::cout<<"Vezi si tu cate elem ai\n";
              meniu();
              break;
              }

			 case '3':{
			      int poz;
			      std::cout<<"Introdu pozitia: ";
                  std::cin>>poz;
                  if(  (*aux).removeAt(poz)  )
                    std::cout<<"Succes\n";
                  else
                    std::cout<<"Vezi si tu cate element ai -_-\n";
                  meniu();
                  break;
              }

			 case '4':{
               int poz;
			   std::cout<<"Introdu pozitia: ";
			   std::cin>>poz;
               std::cout<<"Ce tip de elem adaugi?(Complex/Punct)\n: ";
               std::cin>>tip_el;

			   if(strcmp(tip_el,"Complex")==0)
			   {
				   Object *ad=new Complex;  std::cin>>*ad;
				   int ok=(*aux).insertAt(poz,ad);
				   if(ok)
					   std::cout<<"Adaugat\n";
                   else
                       std::cout<<"Mai ai pana la pozitia aia\n";
			   }
			   else
				    if(strcmp(tip_el,"Punct")==0)
			   {
				   Object *ad=new Punct;  std::cin>>*ad;
				   int ok=(*aux).insertAt(poz,ad);
				   if(ok)
					   std::cout<<"Adaugat\n";
                   else
                       std::cout<<"Mai ai pana la pozitia aia\n";

			   }
			   else
				   std::cout<<"Nedefinit\n";
               meniu();
			   break;
			   }
			 case '5': {  (*aux).RemoveAll(); std::cout<<"Succes!\n"; meniu(); break; }
			 case '6': { std::cout<<*arr<<"Spor\n"; return 0;}
			 default:  { std::cout<<"Cumpara-ti ochelari!!\n"; meniu(); }

		 }
	  }
	  arr=aux;
	  std::cout<<"Lista finala:\n"<<(*arr);
	}
	else
	std::cout<<"Spor!\n";

    return 0;
}
