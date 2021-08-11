import optuna
import subprocess

def objective(trial):
    A = trial.suggest_uniform('A', 0.001, 10000)
    B = trial.suggest_uniform('B', 0.001, A)
    C = trial.suggest_uniform('C', 0.001, 10000)
    D = trial.suggest_uniform('D', 0.001, C)
    sh = f'./tools/run.sh -p -A {A} -B {B} -C{C} -D{D}'
    proc = subprocess.run(sh, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    score = int(proc.stdout.decode('utf8'))
    print(f'score:{score}, A:{A}, B:{B}, C:{C}, D:{D}')
    return score
study = optuna.create_study()
study.optimize(objective, n_trials=10)
print(study.best_params)
print(study.best_value)