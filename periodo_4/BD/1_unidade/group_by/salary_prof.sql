select
	AVG(cargo.salario)
from
	universidade.professor
join
	universidade.cargo ON cargo.id_cargo = professor.cargo
join
	universidade.departamento ON departamento.cod_depto = professor.departamento
group by 
	departamento.cod_depto;

-- Media de salario por departamento 
