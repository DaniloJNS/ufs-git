SELECT 
	* 
FROM 
	universidade.disciplina
INNER JOIN
	universidade.turma ON turma.cod_disc = disciplina.cod_disc

