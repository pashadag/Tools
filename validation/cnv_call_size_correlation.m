function [] = cnv_call_size_correlation(fn,xlim,ylim)

    [ ol c1 s1 e1 l1 r1 c1 s2 e2 l2 r2 ] =  textread(fn, '%u %s %u %u %u %s %s %u %u %u %s');

    [ sortedl1 order ] = sort(l1);
    plot(sortedl1,l2(order),'.'); hold on;
    m = max([ max(l1) max(l2) 1 ]);
    
    out_fn = [ fn '..eps' ];
   
   if xlim == 0
        axis([0 m 0 m]);
        plot([0 m],[0 m]);
   else
        axis([0 xlim 0 ylim]);
        plot([0 xlim],[0 ylim]);
        out_fn = [ fn '.' num2str(xlim) '.eps' ]
   end

    title(fn);
    
    xlabel('our predictions')
    ylabel('their predictions')
    grid on
    
    %out_fn = [ fn '.eps' ];

    print('-depsc', out_fn,'-r300');
    
    exit
end
