- [ ] How to initialise the voronoi points?  
    This can be sampled based on the probability distribution formed by the pixel luminance.    

    Paper proposes the use of rejection sampling which is basically sampling a bunch of pixels initially using 
    any distribution function (`uniform` for eg.) and compute the pdf the based on some selected distribution 
    (scaled `uniform` or `gaussian`), if that pdf is lesser than the pixel darkness (or luminance) then we 
    accept it otherwise reject.

    Pros:
    - Easy to implement.
    - Works quite fine, follows the probability distribution quite well with "cutting corners."  

    Cons:
    - Acceptance rate might not be high, might require quite a lot of samples to get the required number of initial points.

- [ ] How to get rid of the dots outside the "drawing" region?
