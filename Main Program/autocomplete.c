# include <string.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;

int string_compare(const void *t1, const void *t2){
        const term *term1 = t1;
        const term *term2 = t2;
        return strcmp(term1->term, term2->term);
    }

int rank_compare(const void *weight1, const void *weight2)
{
    const term *rank1 = weight1;
    const term *rank2 = weight2;
    return rank2->weight - rank1->weight;
}

void reduce_string(char *substr, struct term *terms, char *string, int mid)
{
    int i;
    char reduced_string[200];
    strcpy(reduced_string, string);

    for (i = 0; i < strlen(substr); i++){
        reduced_string[i] = (terms[mid].term)[i];
    }
    reduced_string[i] = '\0';

    strcpy(string, reduced_string);
}

void create_string(char* s1, char *s2, char* s3, char *s4, char *s5, char *s6, char *result)
{
    if(strcmp(s2, "") == 0){
        strcpy(result, s1);
    }else if(strcmp(s3, "") == 0){
        strcpy(result, strcat(strcat(s1, " "), s2));
    }else if(strcmp(s4, "") == 0){
        strcpy(result, strcat(strcat(s1, " "), strcat(strcat(s2, " "), s3)));
    }else if(strcmp(s5, "") == 0){
        strcpy(result, strcat(strcat(s1, " "), strcat(strcat(s2, " "), strcat(strcat(s3, " "), s4))));
    }else if(strcmp(s6, "") == 0){
        strcpy(result, strcat(strcat(s1, " "), strcat(strcat(s2, " "), strcat(strcat(s3, " "), strcat(strcat(s4, " "), s5)))));
    }else{
        strcpy(result, strcat(strcat(s1, " "), strcat(strcat(s2, " "), strcat(strcat(s3, " "), strcat(strcat(s4, " "), strcat(strcat(s5, " "), s6))))));
    }
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    char line[200];
    FILE *fp = fopen(filename, "r");

    char *str_nterms = fgets(line, sizeof(line), fp);
    int n_terms = atoi(str_nterms);
    *pnterms = n_terms;

    char search_term[200];
    char t[100], t2[100], t3[100], t4[100], t5[100], t6[100], r[100];
    double wght;
   
    (*terms) = (term *)malloc(n_terms * sizeof(term));

    for (int i = 0; i < n_terms; i++){
        
        sscanf(fgets(line, sizeof(line), fp), "%s" "%s" "%s" "%s" "%s" "%s" "%s", r, t, t2, t3, t4, t5, t6);
        
        create_string(t, t2, t3, t4, t5, t6, search_term);

        wght = atof(r);

        strcpy((*terms)[i].term, search_term);
        (*terms)[i].weight = wght; 

        strcpy(t2, "");
        strcpy(t3, "");
        strcpy(t4, "");
        strcpy(t5, "");
        strcpy(t6, "");
    }

    qsort(*terms, n_terms, sizeof(term), string_compare);
}

int lowest_match(struct term *terms, int nterms, char *substr)
{
    int high = nterms;
    int low = 0;
    char reduced_term[200];

    reduce_string(substr, terms, reduced_term, low);
    if(strcmp(substr, reduced_term) == 0){
        return low;
    }

    while (low != high){

        int mid = (high + low)/2;

        reduce_string(substr, terms, reduced_term, mid);

        if(strcmp(substr, reduced_term) == 0){
            reduce_string(substr, terms, reduced_term, (mid - 1));
            if(strcmp(substr, reduced_term) == 0){
                high = mid;
            }else{
                return mid;
            } 
        }else if(strcmp(substr, reduced_term) < 0){
            high = mid - 1;
        }else if(strcmp(substr, reduced_term) > 0){
            low = mid + 1;
        }
    }
    return -1;
}

int highest_match(struct term *terms, int nterms, char *substr)
{
    int high = nterms;
    int low = 0;
    char reduced_term[200];

    reduce_string(substr, terms, reduced_term, high);
    if(strcmp(substr, reduced_term) == 0){
        return high;
    }

    while (low != high){

        int mid = (high + low)/2;

        reduce_string(substr, terms, reduced_term, mid);

        if(strcmp(substr, reduced_term) == 0){
            reduce_string(substr, terms, reduced_term, (mid + 1));
            if(strcmp(substr, reduced_term) == 0){
                low = mid;
            }else{
                return mid;
            }
        }else if(strcmp(substr, reduced_term) < 0){
            high = mid - 1;
        }else if(strcmp(substr, reduced_term) > 0){
            low = mid + 1;
        }
    }
    return -1;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    int max_index = highest_match(terms, nterms, substr);
    int min_index = lowest_match(terms, nterms, substr);
    *n_answer = (max_index - min_index + 1);

    (*answer) = (term *)malloc(*n_answer * sizeof(term));

    int j = 0;
    for(int i = min_index; i <= max_index; i++){
        strcpy((*answer)[j].term, terms[i].term);
        (*answer)[j].weight = terms[i].weight;
        j++;
    }

    qsort(*answer, *n_answer, sizeof(term), rank_compare);
}


void main()
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    
    int l_index;
    int h_index;

    int n_words;
    char input[100];
    printf("How many words would you like to autocomplete?: ");
    scanf("\n%d", &n_words);

    for(int w = 0; w < n_words; w++){
        printf("\nWhat is the word you want to autocomplete?: ");
        scanf("\n%s", &input);

        l_index = lowest_match(terms, nterms, input);
        h_index = highest_match(terms, nterms, input);

        struct term *answer;
        int n_answer;
        autocomplete(&answer, &n_answer, terms, nterms, input);

        printf("All posible sugestions ranked by number of google searches are: \n");
        for (int ind = 0; ind < n_answer; ind++){

            printf("%s %s", answer[ind].term, " ");
            printf("%lf \n", answer[ind].weight);

        }

        printf("\nThe top autocomplete sugeestion is: %s and it's been searched %lf times\n", answer[0].term, answer[0].weight);

        free(answer);
        
    }
    

    free(terms);
}
