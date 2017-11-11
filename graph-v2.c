    #include<stdio.h>

    #include<stdlib.h>

    #include <time.h>



    #define MAX_VERTICES 900000;

    #define MAX_EDGES MAX_VERTICES-1;



    typedef unsigned char vertex;



    int main(){


        int numberOfVertices =  MAX_VERTICES;

        /*graphs is 2 dimensional array of pointers*/

        if( numberOfVertices == 0)

            numberOfVertices++;

        printf("Total Vertices = %d, Max # of Edges = %d\n",numberOfVertices, numberOfVertices-1);

        FILE *ofs;
        ofs = fopen("g.txt", "w");

        /*start linking the graph. All vetrices need not have same number of links*/

        int vertexCounter = 0;

        for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++){

            fprintf(ofs, "%d", vertexCounter);
          //  printf("%d:\t",vertexCounter);
            //let each node have 2 edges only

                    /*link the vertices*/

                    int linkedVertex;
                    //left nb
                    linkedVertex = (2*vertexCounter) + 1;
                    if(linkedVertex >= numberOfVertices)
                      break;
                    //graph[vertexCounter][0] = graph[linkedVertex];
                    fprintf(ofs, " %d", linkedVertex);

                    linkedVertex = (2*vertexCounter) + 2;
                    if(linkedVertex >= numberOfVertices)
                      break;
                    //graph[vertexCounter][0] = graph[linkedVertex];
                    fprintf(ofs, " %d", linkedVertex);





            fprintf(ofs, "\n");
            //printf("\n");

        }

        return 1;

    }
