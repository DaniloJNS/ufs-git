select
  prof.primeiro_nome nome, prof.sobrenome,
  chef.primeiro_nome chefe_nome, chef.sobrenome chefe_sobrenome
from 
  universidade.professor
  join
	universidade.usuario prof ON prof.cpf = professor.cpf
  join 
	universidade.departamento ON departamento.cod_depto = professor.departamento
  join
	universidade.professor chefe ON chefe.mat_professor = departamento.chefe
  join
	universidade.usuario chef ON chef.cpf = chefe.cpf
  where
	chefe.mat_professor != professor.mat_professor;
