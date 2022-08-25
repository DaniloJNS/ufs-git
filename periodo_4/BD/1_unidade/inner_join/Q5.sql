select 
	disciplina.nome nome, pre_req_disciplina.nome pre_req_nome 
from
	universidade.disciplina
	join 
		universidade.disciplina pre_req_disciplina ON pre_req_disciplina.cod_disc = disciplina.pre_req;
