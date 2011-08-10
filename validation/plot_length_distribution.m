function plot_length_distribution(path_to_callfile)

chr_ind = 1;
start_ind = 2;
end_ind = 3;
count_ind = 4;

figure('Visible','off');

disp('opening file');
fid = fopen(path_to_callfile,'r');
data = textscan(fid, '%s %u %u %d%*[^\n]');

lengths = data{end_ind} - data{start_ind};

%%% EDIT THIS VARIABLE TO CHANGE BIN SIZES %%%
labels = {  1 '1k-2k';
	    2 '2k-4k';
	    4 '4k-8k';
	    8 '8k-16k';
	    16 '16k-32k';
	    32 '32k-64k';
	    64 '64k-128k';
	    128 '128k+'};
% labels = {  1 '1k-2k'; 2 '2k-4k'; 4 '4k-8k'; 8 '8k-16k'; 16 '16k-32k'; 32 '32k-64k'; 64 '64k-128k'; 128 '128k+'};




%seemingly useless code, but afraid to delete for now
%gain_lengths = lengths(data{count_ind} >= 0);
%loss_lengths = lengths(data{count_ind} < 0);

num_bins = size(labels,1);

label_strings = [];

for i = 1:num_bins
    label_strings = [label_strings labels(i,2)];
end

gain_counts = zeros(num_bins,1);
loss_counts = zeros(num_bins,1);

for x = 1:size(data{chr_ind},1)
    
    bin = 0;
    
    for i = num_bins:-1:1
        if lengths(x) >= labels{i,1}*1000
            bin = i;
            break
        end
    end
   
    if bin == 0
        fprintf('no bin appropriate for data point %u (lengths(x): %u)\n',x,lengths(x));
    else
        if data{count_ind}(x) < 0
            loss_counts(bin) = loss_counts(bin) + 1;
        else
            gain_counts(bin) = gain_counts(bin) + 1;
        end
    end
end

disp('drawing plot');
bar([gain_counts loss_counts],'Stack');
legend('gains','losses');
xlabel('length (bp)');
ylabel('number');
grid on;
colormap('gray');
set(gca,'XTickLabel',label_strings);
disp(['outputing pdf file...' path_to_callfile '.lendist.pdf' ]);
print('-dpdf',[ path_to_callfile '.lendist.pdf' ]);
disp([ 'outputing eps file...' path_to_callfile '.lendist.eps' ]);
print('-depsc',[ path_to_callfile '.lendist.eps' ]);
exit
