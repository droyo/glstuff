/* This code is written by Eskil Steenburg, described here
	http://iloapp.quelsolaar.com/blog/news?Home&post=45 
	I have edited some formatting and variable names so
	that it doesn't work right now. It's for looking, not
	using */

int vertex_count = ARBITRARY_VALUE;
float vertex[vertex_count];
int *
compute_neighbor(int *ref, int ref_count)
{
	int i, j;
	int *neighbors = malloc(sizeof(int) * ref_count * 4); // Why 4?
	int *tmp = malloc(sizeof(int) * vertex_count);
	int clear = 0;
	int laps = 0;
	int cor, a, b;
	int tq1, tq2;

	for( i = 0; i < neighbors_count; i++ ) { n[i] = -1; }
	for( i = 0; i < gg_vertex_count; i++ ) { tmp[i] = -1; }

	while(clear < ref_count * 4)
	{
		for(i = 0; i < ref_count * 4; i++)
		{
			if(ref[i] < vertex_count && ref[i + 1] < vertex_count && ref[i + 2] < vertex_count && vertex[ref[i] * 3] != V_REAL32_MAX && vertex[ref[i + 1] * 3] != V_REAL32_MAX && vertex[ref[i + 2] * 3] != REAL32_MAX)
			{
				if(ref[i + 3] < vertex_count && vertex[ref[i + 3] * 3] != REAL32_MAX)
					tq1 = 4;
				else
					tq1 = 3;
	
				for(j = 0; j < tq1; j++)
				{
					cor = v[ref[i]];
					if(cor == -1)
					{
						if(n[i] == -1 || n[(i / 4) * 4 + (i + tq1 - 1) % tq1] == -1)
							v[ref[i]] = i;
					}
					else if(cor == i)
					{
						v[ref[i]] = -1;
					}
					else
					{
						if(ref[((cor / 4) * 4) + 3] < vertex_count && vertex[ref[((cor / 4) * 4) + 3] * 3] != V_REAL32_MAX)
							tq2 = 4;
						else
							tq2 = 3;
						a = (i / 4) * 4;
						b = (cor / 4) * 4;
						if((n[cor] == -1 || n[a + (i + tq1 - a - 1) % tq1] == -1) && ref[a + (i + tq1 - a - 1) % tq1] == ref[b + (cor + 1 - b) % tq2])
						{
							n[a + (i + tq1 - a - 1) % tq1] = cor;
							n[cor] = a + (i + tq1 - a - 1) % tq1;
							clear = 0;
							if(n[b + (cor + tq2 - 1) % tq2] != -1)
							{
								if(n[i] == -1)
									v[ref[i]] = i;
								else
									v[ref[i]] = -1;
							}
						}
						if((n[i] == -1 || n[b + (cor + tq2 - b - 1) % tq2] == -1) && ref[a + (i - a + 1) % tq1] == ref[b + (cor + tq2 - b - 1) % tq2])
						{
							n[i] = b + (cor + tq2 - b - 1) % tq2;
							n[b + (cor + tq2 - b - 1) % tq2] = i;
							clear = 0;	
							if(n[cor] != -1)
							{
								if(n[a + (i + tq1 - a - 1) % tq1] == -1)
									v[ref[i]] = i;
								else
									v[ref[i]] = -1;
							}
						}
					}
					i++;
				}
				i = i + 4 - tq1;
			}else
				i += 4;
			clear++;
		}
	}
	free(v);
	return n; 
}
	
