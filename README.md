# Always-Less-Effort
Method that allows you to compute a schedule to achieve your goal with less and less effort every day.  
User-defined parameters allow you to set the rate between the first day and the last day workloads still retaining the goal to complete the overall workload.

Copyright 2026 Samuel Bresciani bresciani.app@gmail.com

## What is it

It often happens that a job requires a progressive obtainment of results, that are cumulated day per day in function of previous days results.
It can happen due to the final scope of the job or due to the nature itself of the process, full of check or review phases, each time more complex, 
that usually brings to a situation in which the work to handle on last days, close to the deadline, results really harder compared to the work of first days.

The scope of the Always-Less-Effort method is to organise the daily work in a user-controlled decreasing way, instead of a constant scheduling, through a personalised parameter 
which permits to accumulate more work on first days and less on last days close to deadline, 
in order to reach the end of the process having more available time to manage the overall results of previous days and eventual unforeseen obstacles.

In terms of computational speed, an important advantage in using this method is that the parameters are computed just once with a single iteration, thanks to an analytical solution,
and they are not deduced with an iterative process, as instead other algorithms could do. This is possible thanks to the exact solution of trend parameters, as illustrated in the section [How to use](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#how-to-use)
and mathematically explained in the section [Analytical solution demonstration](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#analytical-solution-demonstration).


## Generic example
We can suppose to have an hypothetical building process of a final product, that requires some check phases in the middle of the overall process (to be sure of
results obtained till that moment) or maybe a comprehensive final phase, and we want to avoid an effort accumulation close to the deadline or manage the best possible the situation in case some quality check failed and hence some past phase has to be repeated.

Hence, supposing to have an overall of 200 phases for the building process and a deadline of 10 days to realise the product, if we considered a constant daily work we would have a schedule of about 20 process phases per day

| Day | Daily work phases  |
|:----|:------------------:|
| 1   |         20         |
| 2   |         20         |
| 3   |         20         |
| 4   |         20         |
| 5   |         20         |
| 6   |         20         |
| 7   |         20         |
| 8   |         20         |
| 9   |         20         |
| 10  |         20         |

![Constant schedule](Constant_schedule.png "Constant schedule")

Using instead the Always-Less-Effort method, setting the input parameter of rate as **50** ( which means 50% more of work on first day compared to the last day ) and of course **10** available days parameter and **200** generic processes as overall workload,
we can obtain a decreasing daily work schedule that considers all the 200 phases in 10 days but takes also into account our desired initial increment of work expressed through the rate of work between last day and first one.  
The optimal value of this rate can be deduced thanks to the personal experience on how much we think last phases of a process could be harder than first phases,
or simply as an attempt to have more time during last days to easier handle the overall production process and relative checks or reviews.  
(for an illustration of algorithm usage look at [how to use](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#how-to-use) section)

| Day | Daily work phases  |    Approximated value of daily work phases    |
|:----|:------------------:|:---------------------------------------------:|
| 1   |         24         |                      24                       |
| 2   |       23.11        |                      23                       |
| 3   |       22.22        |                      22                       |
| 4   |       21.33        |                     21.5                      |
| 5   |       20.44        |                     20.5                      |
| 6   |       19.56        |                     19.5                      |
| 7   |       18.67        |                     18.5                      |
| 8   |       17.78        |                      18                       |
| 9   |       16.89        |                      17                       |
| 10  |         16         |                      16                       |

For the consistence of the method the sum of daily work phases is obviously the overall workload of 200 phases.

![ALE method decreasing schedule](ALE_schedule.png "ALE method decreasing schedule")


Approximated values of daily work phases are computed too and these are checked to be consistent with the total of 200 phases (the sum of the column values has to be the input goal of 200 phases). 
In the particular case it results that the sum of approximated values is not exactly consistent with the overall workload the user is warned in the output with a specific parameter.


## Areas of application
In every context in which the daily activity is every day more and more complex, due to its cumulative nature 
or simply because it is based on previously days progresses, this method can be applied.
Here two possible examples are illustrated:
- in a company it often happens that a worker, whether for its own job or to manage the jobs of the subordinates, has to organise the work productivity
in function of a deadline where the final result complexity considers all overall results obtained during previous activities, and it maybe needs a continue testing action,
which leads necessary to accumulate working complexity to days close to deadline. Handling the working schedule in a linear decreasing controlled way it can allow to have more time available on last days to handle last checks and eventually manage better unforeseen events.

- in didactic context, for a better organisation of study material, this method can be useful for a student that can consider the total number of pages to study for preparing an exam as the equivalent of the workload mentioned above (e.g. 200 pages instead of 200 work phases);
in general to prepare an exam if you take into account a certain number of pages studied during first days (e.g.: 10 first pages of study material to prepare an exam) these are really easier than the same number of pages studied during last days (10 last pages of study material) 
because the study concepts are often based on progressive knowledge acquired by studying day by day; 
therefore, if the student considers just a constant study schedule of 10 pages a day, the study process could result much harder during last days. 
With a decreasing controlled method instead the student can organise the study schedule for having less pages to study during last days, thanks to a parameter chosen by the student in function of subject complexity, 
avoiding that the ending accumulation of study material brings to a stressful situation, leading instead to the final exam with a more rested mind.
That's how this method was born and has been applied efficiently during Bachelor and Master Degree studies.
For this usage it exists an **Android app** ( [StudySprint](https://play.google.com/store/apps/details?id=com.studysprint.app) close to the official release ), downloadable from PlayStore for free, that also helps the student with an updating schedule feature in case of falling behind in the study deadlines.

You have to consider that in every situation the reduction of work ( or study ) on last days implies inevitably the increasing of first days work ( or study ) for the obvious necessity to achieve the total workload ( or study ) material; 
and this aspect can be managed thanks to the designated parameter of the rate between the work ( or study ) on first day with compared to the last day. 

## How to use
The scripts are written in C++ 17.  
It is enough to execute the _main_ to obtain an example of possible usage of the algorithm.

The algorithm parameters refer to the case in which we consider the number of phases of a production process as a concept of overall workload, 
but it can obviously be directly used for every area of application is needed, just by quantifying the overall workload through the parameter _nPhases_.

The input parameters of the algorithm are:
- **nPhases** as number of phases needed to obtain the final product
- **nDays** as number of available days before the deadline
- **desiredRate** the desired work increase, in terms of percentage, of first day compared to the last day.

(the underlying mathematical logic is illustrated in the section [main parameters of the algorithm one-iteration solution](https://github.com/BrescianiS/AlwaysLessEffort_method#main-parameters-of-the-algorithm-one-iteration-solution))  
Decimal number of days can be entered too (e.g. 12.3); in this case the algorithm will organise the computation, and the output text too, managing all in terms of half days.  
The optimal value of rate parameter _desiredRate_ can be deduced as illustrated in the [generic example](https://github.com/BrescianiS/AlwaysLessEffort_method#generic-example), in function of the experience on how harder could be the last phases compared to first ones.

Additional input parameters can be set for the precision of the algorithm in checking how much the computed values are near to analytical solution and for output text values writing approximation too.
These out-of-precision cases are really rare situations but their self-correction has been considered for completeness
(their functionality is illustrated in [self-correction procedure](https://github.com/BrescianiS/AlwaysLessEffort_method#self-correction-procedure) section).

The repository consists of two libraries:
- **_AlwaysLessEffort_** that elaborates the actual algorithm on which the method is based
- **_TextCreator_** which creates the output text content with all informations about the obtained results

Both are called in the main as a practical example.  
The output values of _AlwaysLessEffort_ are all the computed values of the algorithm and eventual warning.  
The _TextCreator_ class takes as input a AlwaysLessEffort class variable and additional output approximation parameters and then it creates the string of the content of the output text file.  
An example of the final text file as result of the overall elaboration is reported in repository files. 


## Analytical logic

### Main parameters of the algorithm one-iteration solution
An important advantage of this method, in terms of computational speed, is that its parameters are computed in an analytical exact way in one single computation, and they are not obtained with more converging iterations as some algorithms do. 
This is possible thanks to an analytical solution of the problem, as demonstrated in [formulas demonstrations](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#analytical-solution-demonstration) section, that had led to the parameters illustrated in this section.

The workload daily distribution is computed as follows:

calling $R$ the previously introduced rate between the 'quantity' of work to perform on first day and on the last day,  
a distribution of daily work (here in terms of work phases) can be defined as $p(d) = q - m d$,  
where $p(d)$ is the number of work phases to be performed on the day $d$, with $m$ and $q$ two parameters positively defined, analytically computed in function of total available days $N$ (as 10 days in the [generic example](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#generic-example))
and the overall work to be completed $P$ (as 200 total work phases in the [generic example](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#generic-example)).

The two main algorithm parameters are

$q = \frac{2 P R}{ N ( R +1) } $

$m = q \frac{ R -1}{ R (N - 1) }$

Hence, once computed _q_ and _m_ in function of the input parameters (_P_, _N_ and _R_), the value of number of phases $p(d)$ to be completed on a day $d$ can be easily obtained as $p(d) = q - m d$.  
Please take into account that the number of days goes from 0 up to to _N-1_.  
Alternatively we can start computing the number of phases of first day ( $p(0)$ ) that is exactly $q$, then all values of number of phases to be carried out on following days can be easily obtained in function of previous day value as $p(d) = p(d-1) - m$. 

The origins of these formulas and relative mathematical demonstrations are all illustrated in [formulas demonstrations](https://github.com/BrescianiS/AlwaysLessEffort_method?tab=readme-ov-file#analytical-solution-demonstration).


### Self-correction procedure
For the sake of completeness the self-correction procedure has been considered for the cases in which some of the obtained results 
depart from the requested value due to the computational precision. This is checked through default precision parameters, settable also from input as secondary parameters for _AlwaysLessEffort_.

In case the rate between first and last day computed phases ( $p(0) / p(N-1)$ ) results far from the desired value ( _desiredRate_, as _R_ in the formulas ) more than the precision criteria ( default: 0.0001 $\equiv$ 0.01% of increase )
or the sum of computed phases $p(d)$ does not result enough near to the input value ( _nPhases_, as _P_ in the formulas ) using the precision criteria ( default: 0.01 phases ) ,
a self-correction procedure takes action which variates the parameters value of $q$ and $m$ rescaling them in function of the desired overall work _P_.  
It works actually like this: initially _m_ is scaled in function of the rate between desired and obtained _P_  
( $m^{'} = m \frac{P}{P^* }$ with _m'_ the new _m_ and _P*_ the sum of all the obtained phases $p(d)$ )  
and _q_ is computed in function of the linear formula that relates _m_ and _q_ in function of _P_ and _N_  
( $q^{'} = \frac{ P + m^{'} (N (N-1)/2) }{N}$ where _q'_ is the new _q_ )  
then, if the computed value of _R_ ( $R^{'} = \frac{q^{'} }{q^{'} - m^{'} (N-1)}$ ) is considered acceptable into a certain range of values ( not far from desired _R_ considering the double of its precision criteria), 
the new _m_ and _q_ values are accepted and the standard process for schedule computation can start again from day 0 with new corrected _m_ and _q_ values, otherwise the procedure is refused and the process stops.  
With this logic the correction on the total number of phases $P$ has been considered to have priority over the correction on the rate $R$.  
In case after the end of the schedule computation the final values of _P_ or _R_ were again still not enough near to the desired ones, this self-correction process can be repeated, for a maximum number of times defined through a secondary parameter ( default: 2 ).

### Analytical solution demonstration
A decreasing linear trend has been considered for the schedules, therefore naming $p(d)$ the phases to be completed on a day $d$,
it can be thought that on the first day ( that corresponds to $d = 0$ ) the number of phases to complete are exactly $q$ due to the linear formula $p(d) = q - m d$ ; 
therefore if the first day is $d=0$ then the last available day that the algorithm can consider is $d = (N -1)$ because they are a total of $N$.

Considering the overall work to be completed, named $P$, as the sum of all daily works $p(d)$:  
$P = \sum_{d=0}^{N -1} p(d) = p(0) + p(1) + ... + p(N-1) = q + \sum_{d=1}^{N -1} p(d)$  
where every day work decreases of a value _m_ compared to the previous one, from day $0$ to $N - 1$, so  
$P = q + (q-m) + ( q -m -m) + ( q -m -m -m) + ... + (q - m(N-1))$
$= N q - m \sum_{d=1}^{N-1} d = N q - m \frac{N (N-1)}{2} $

The rate $R$ between the work on first day and last day can be computed as  
$R = \frac{p(0)}{p(N-1)} = \frac{q}{ q - m(N-1)}$ , with _R_ > 1 for the scope itself of the algorithm.

Therefore, two equations have been obtained to relate $P$, $R$ and $N$ to $m$ and $q$:

$P = N q - m \frac{N (N-1)}{2} $  
$R = \frac{q}{ q - m(N-1)}$

Considering we know $N$, $R$ and $P$ values because they are defined as input parameters, we can obtain $q$ and $m$ values:

$q = \frac{P/N}{1 - (1/2) (1 - 1/R)}  = \frac{2 P R}{ N ( R +1) }  $  
$m = q \frac{ 1 - 1/R}{ N - 1} = q \frac{ R -1}{ R (N - 1) } = \frac{2 P (R -1)}{ N (N-1) ( R +1) } $.

With this logic the daily schedule has been computed in an analytically exact way without repetitive converging iterations, making easier
to handle all the workload before the deadline, according to personally chosen parameters.



The algorithm logics could be generalised to every contest of non-linear trend as long as computable in a continuous form that lead to an analytically solvable solution.  
Probably with a second-degree trend since there are three input parameters but more constraints or considerations have to be done.
Following the same logic used below with a second-degree decreasing realistic function ( $p(d) = -a d^{2} - b d + c$ ) we can find these relations:  
$P = N c - b \frac{N(N-1)}{2} - a \frac{N( N-1)(2N -1)}{6}$  
$R = c /( -a (N-1)^2 - b (N-1) + c) $  
but an additional relation is necessary to find $a$, $b$ and $c$.  
Maybe it could work for more degrees too but other practical considerations have to be applied for specific cases.

On the same way if it was used an iterative converging process the schedule could be computed with almost all non-uniform trends,
but it would obviously loose the computational benefit of an analytically exact formula that does not require converging iterations and obtains the precise solution as indeed this method does. 
