#ifndef NTREE_H
#define NTREE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define N 20 // any node can have at most 20 children


template <class T>
class Ntree
{
    public:

        struct tnode
        {
            T value;
            tnode* children[N];

            // constructor assings to 'value' and
            // initializes 'children' array to nulls
            tnode(const T& val): value(val){
                for(int i = 0; i<N; ++i)
                    children[i] = 0;
            }


            tnode* addChild(const T& val)
            {
                tnode* child = new tnode(val); // make new node with 'value' val
                // add the new node to the 'children' array in the first open (null) spot
                for(int i = 0; i<N; ++i)
                {
                    if(children[i] == 0)
                    {
                        children[i] = child;
                        break;
                    }
                }

                return child;
            }

        };
        // default constructor
        Ntree(): root(0){}
        // constructor, makes new node with val, assigns to root
        Ntree(const T& val): root(new tnode(val)){}

        tnode* getRoot(){ return root;}

        // used stringstreams to convert everything to strings
        std::string toString()
        {
           std::stringstream str; // make ss object
           toStringRec(root, str); // call recursive function
            return str.str(); // return string function of ss object
        }

        // recursive function to put everything into ss object
        void toStringRec(tnode* root, std::stringstream& str)
        {
            if(!root) return;

            str << root->value << std::endl;
            for(int i=0; i<N && root->children[i]; ++i)
            {
               toStringRec(root->children[i], str);
            }

            str << "#" << std::endl; // marker to divide sets of children with their parents
        }


        bool operator==(const Ntree<T> rhs)
        {
            return root->value == rhs.root->value;
        }

        // passes outfile and root to recursive function
        void serialize (const char* filepath)
        {
            std::ofstream outfile(filepath);
            serializeRec(root, outfile);
            outfile.close();
        }

        // recrusively serializes
        void serializeRec(tnode* root, std::ofstream& outfile)
        {
            if(!root) return;

            outfile << root->value << std::endl;
            for(int i=0; i<N && root->children[i]; ++i)
            {
               serializeRec(root->children[i], outfile);
            }

            outfile << "#" << std::endl;

        }

        // passes infile and root to recursive function
        void deserialize(const char* filepath)
        {
            std::ifstream infile(filepath);
            deserializeRec(root, infile);
            infile.close();
        }

        // recursively deserializes
        int deserializeRec(tnode* &root, std::ifstream& infile)
        {
            std::string val;
            if(!(infile >> val) || val == "#")
                return 1;

            else
            {
                root = new tnode(val);
                for(int i = 0; i<N; ++i)
                    if(deserializeRec(root->children[i], infile))
                        break;
            }

            return 0;
        }

    private:
        tnode* root;
};


#endif // NTREE_H
